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

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;
layout(location=8) in vec4 TexCoord;

out Attribs {
    vec4 couleur;
    vec3 normale, lumiDir[3], obsVec;
    vec2 texCoord;
} AttribsOut;

uniform float temps;

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
        coul += attenuation * FrontMaterial.diffuse * LightSource.diffuse[j] * NdotL;

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
    // transformation standard du sommet
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // calculer la normale qui sera interpolée pour le nuanceur de fragments
    vec3 N = matrNormale * Normal ;

    // calculer la position du sommet (dans le repère de la caméra)
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // calculer le vecteur de la direction de la lumière (dans le repère de la caméra)
    vec3 L[3];

    for (int i = 0; i < 3; i++) {
        L[i] = ( matrVisu * LightSource.position[i] ).xyz - pos;
    }

    // calculer le vecteur de la direction vers l'observateur
    vec3 O = -pos;

    vec4 coul = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;

    if (typeIllumination == 0) {
        N = normalize( N );

        for ( int i = 0; i < 3; i++ ) {
            L[i] = normalize( ( matrVisu * LightSource.position[i] ).xyz - pos );
        }

        O = normalize( O );

        for ( int j = 0; j < 3; j++ ) {
            coul += calculerReflexion( j, L[j], N, O );
        }
    }

    AttribsOut.normale = N;
    AttribsOut.lumiDir = L;
    AttribsOut.obsVec = O;
    
    // assigner la couleur du sommet
    AttribsOut.couleur = clamp( coul, 0.0, 1.0 );

    // transmettre au nuanceur de fragments les coordonnées de texture reçues
    AttribsOut.texCoord.s = TexCoord.s - 0.1 * temps;
    AttribsOut.texCoord.t = TexCoord.t;
}
