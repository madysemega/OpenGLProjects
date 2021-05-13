#version 410

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient[3];
    vec4 diffuse[3];
    vec4 specular[3];
    vec4 position[3];      // dans le repère du monde
} LightSource;

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante globale
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

layout (std140) uniform varsUnif
{
    // partie 1: illumination
    int typeIllumination;     // 0:Gouraud, 1:Phong
    bool utiliseBlinn;        // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
    bool afficheNormales;     // indique si on utilise les normales comme couleurs (utile pour le débogage)
    // partie 2: texture
    int iTexCoul;             // numéro de la texture de couleurs appliquée
    // partie 4: texture
    int iTexNorm;             // numéro de la texture de normales appliquée
};

uniform sampler2D laTextureCoul;
uniform sampler2D laTextureNorm;

/////////////////////////////////////////////////////////////////

in Attribs {
    vec4 couleur;
    vec3 normale, lumiDir[3], obsVec;
    vec2 texCoord;
} AttribsIn;

out vec4 FragColor;

float attenuation = 1.0;
vec4 calculerReflexion( in int j, in vec3 L, in vec3 N, in vec3 O ) // pour la lumière j
{
    // calculer la composante ambiante pour la source de lumière
    vec4 coul = FrontMaterial.ambient * LightSource.ambient[j];

    // calculer l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calculer la composante diffuse
        coul += FrontMaterial.diffuse * LightSource.diffuse[j] * NdotL;

        // calculer la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = ( utiliseBlinn ?
                       dot( normalize( L + O ), N ) : // dot( B, N )
                       dot( reflect( -L, N ), O ) ); // dot( R, O )
        if ( spec > 0 ) coul += FrontMaterial.specular * LightSource.specular[j] * pow( spec, FrontMaterial.shininess );
    }
    return( coul );
}

void main( void )
{
    vec4 coul = AttribsIn.couleur;

    // aller chercher (échantilloner) la couleur du fragment dans la texture
    vec4 tex = texture( laTextureCoul, AttribsIn.texCoord );

    if (iTexCoul != 0 && length( tex.rgb ) < 0.5) {
        discard;
    }

    if (typeIllumination == 1) {
        vec3 L[3]; // vecteur vers la source lumineuse
        vec3 N = normalize( AttribsIn.normale ); // vecteur normal
        vec3 O = normalize( AttribsIn.obsVec );  // position de l'observateur

        for (int i = 0; i < 3; i++) {
            L[i] = normalize( AttribsIn.lumiDir[i] );
            coul += calculerReflexion( i, L[i], N, O );
        }
    }
    FragColor = clamp( coul, 0.0, 1.0 );

    if (iTexCoul != 0) {
        FragColor *= tex;
    }
}
