#version 410

////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
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
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////

uniform int illumination; // on veut calculer l'illumination ?

const bool utiliseBlinn = true;

in Attribs {
    vec4 couleur;
    vec3 lumiDir, normale, obsVec;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    // la couleur du fragment est la couleur interpolée
    if(illumination == 0) {
        FragColor = AttribsIn.couleur;
    }else{
        vec3 L = normalize(AttribsIn.lumiDir);
        vec3 N = normalize(gl_FrontFacing ? AttribsIn.normale : -AttribsIn.normale);
        vec3 O = normalize(AttribsIn.obsVec);
        vec4 coul = FrontMaterial.emission;
        coul += FrontMaterial.ambient * LightSource.ambient;
        float NdotL = max(0.0, dot(N, L));
        if (NdotL > 0.0)
        {
            coul += FrontMaterial.diffuse * NdotL * LightSource.diffuse * AttribsIn.couleur;
            float spec = (utiliseBlinn ? dot(normalize(L + O), N) : dot(reflect(-L, N), O));
            if (spec > 0){
            coul += FrontMaterial.specular * LightSource.specular * pow(spec, FrontMaterial.shininess);
        }
        FragColor =  clamp(coul, 0.0, 1.0);
    }
    }
}
