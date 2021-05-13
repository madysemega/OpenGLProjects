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

uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(quads) in;

in Attribs {
    vec3 normale;
    vec2 texCoord;
    vec4 Vertex;
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec3 normale, lumiDir[3], obsVec;
    vec2 texCoord;
} AttribsOut;

uniform mat4 matrModel;
uniform bool deformer;

float interpole( float v0, float v1, float v2, float v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    float v01 = mix( v0, v1, gl_TessCoord.x );
    float v32 = mix( v3, v2, gl_TessCoord.x );
    return mix( v01, v32, gl_TessCoord.y );
}
vec2 interpole( vec2 v0, vec2 v1, vec2 v2, vec2 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec2 v01 = mix( v0, v1, gl_TessCoord.x );
    vec2 v32 = mix( v3, v2, gl_TessCoord.x );
    return mix( v01, v32, gl_TessCoord.y );
}
vec3 interpole( vec3 v0, vec3 v1, vec3 v2, vec3 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec3 v01 = mix( v0, v1, gl_TessCoord.x );
    vec3 v32 = mix( v3, v2, gl_TessCoord.x );
    return mix( v01, v32, gl_TessCoord.y );
}
vec4 interpole( vec4 v0, vec4 v1, vec4 v2, vec4 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec4 v01 = mix( v0, v1, gl_TessCoord.x );
    vec4 v32 = mix( v3, v2, gl_TessCoord.x );
    return mix( v01, v32, gl_TessCoord.y );
}

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

void main( void ) {
    AttribsOut.normale = interpole( AttribsIn[0].normale, AttribsIn[1].normale, AttribsIn[3].normale, AttribsIn[2].normale);
    AttribsOut.texCoord = interpole( AttribsIn[0].texCoord, AttribsIn[1].texCoord, AttribsIn[3].texCoord, AttribsIn[2].texCoord);
    vec4 vertex = interpole(AttribsIn[0].Vertex, AttribsIn[1].Vertex, AttribsIn[3].Vertex, AttribsIn[2].Vertex);
    gl_Position = matrProj * matrVisu * matrModel * vertex;

    vec3 N = normalize( AttribsOut.normale);
    vec3 pos = vec3(matrVisu * matrModel * vertex);
    vec3 L[3];
    for (int i = 0; i < 3; ++i) {
        L[i] = normalize((matrVisu * LightSource.position[i]).xyz - pos);
        AttribsOut.lumiDir[i] = (matrVisu * LightSource.position[i]).xyz - pos;
    }
    AttribsOut.obsVec = -1.0 * pos;
    vec3 O = normalize( AttribsOut.obsVec);
    if (typeIllumination == 0) {
        for (int i = 0; i < 3; i++) {
            AttribsOut.couleur += calculerReflexion(i, L[i], N, O);
        }
        AttribsOut.couleur = clamp(AttribsOut.couleur, 0.0, 1.0);
    }
    if (deformer) 
    {
        gl_Position.z = 1.0 - length(gl_Position.xy);
        gl_Position = matrModel * gl_Position;
    }
}