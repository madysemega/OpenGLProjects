#version 410

uniform vec3 bDim, posPuits;
uniform float temps, dt, tempsDeVieMax, gravite;

layout(location=6) in vec3 Vertex;
layout(location=7) in vec4 Color;
layout(location=8) in vec3 vitesse;
layout(location=9) in float tempsDeVieRestant;

out vec3 VertexMod;
out vec4 ColorMod;
out vec3 vitesseMod;
out float tempsDeVieRestantMod;

uint randhash( uint seed ) // entre  0 et UINT_MAX
{
    uint i=(seed^12345391u)*2654435769u;
    i ^= (i<<6u)^(i>>26u);
    i *= 2654435769u;
    i += (i<<5u)^(i>>12u);
    return i;
}
float myrandom( uint seed ) // entre  0 et 1
{
    const float UINT_MAX = 4294967295.0;
    return float(randhash(seed)) / UINT_MAX;
}

void main( void )
{
    // se préparer à produire une valeur un peu aléatoire
    uint seed = uint(temps * 1000.0) + uint(gl_VertexID);
    // couleur aléatoire par interpolation linéaire entre COULMIN et COULMAX
    const float COULMIN = 0.2; // valeur minimale d'une composante de couleur lorsque la particule (re)naît
    const float COULMAX = 0.9; // valeur maximale d'une composante de couleur lorsque la particule (re)naît

    if ( tempsDeVieRestant <= 0.0 )
    {
        // faire renaitre la particule au puits
        VertexMod = posPuits;
        // assigner un vitesse aléatoire
        vitesseMod = vec3( mix( -25.0, 25.0, myrandom(seed++) ),   // entre -25 et 25
                           mix( -25.0, 25.0, myrandom(seed++) ),   // entre -25 et 25
                           mix(  25.0, 50.0, myrandom(seed++) ) ); // entre  25 et 50
        //vitesseMod = vec3( -20.0, 0., 50.0 );

        // nouveau temps de vie aléatoire
        tempsDeVieRestantMod = myrandom(seed++) * tempsDeVieMax; // à modifier pour une valeur aléatoire entre 0 et tempsDeVieMax secondes
        ColorMod = vec4( 1.0, 1.0, 1.0, 1.0 ); // composantes de couleur
    }
    else
    {
        // avancer la particule (méthode de Euler)
        VertexMod = Vertex + dt * vitesse;
        vitesseMod = vitesse;

        // diminuer son temps de vie
        tempsDeVieRestantMod = tempsDeVieRestant - dt;

        // garder la couleur courante
        ColorMod = Color;

        // collision avec la demi-sphère ?
        vec3 posSphUnitaire = VertexMod / bDim;
        vec3 vitSphUnitaire = ( vitesseMod * bDim ) / 2;

        float dist = length ( posSphUnitaire );
        if ( dist >= 1.0 ) // ... la particule est sortie de la bulle
        {
            VertexMod = ( 2.0 - dist ) * VertexMod;
            vec3 N = posSphUnitaire / dist;  // normaliser N
            vec3 vitReflechieSphUnitaire = reflect ( vitSphUnitaire , N );
            vitesseMod = vitReflechieSphUnitaire / bDim;

            ColorMod = vec4( mix( COULMIN, COULMAX, myrandom( seed++ ) ),
                             mix( COULMIN, COULMAX, myrandom( seed++ ) ),
                             mix( COULMIN, COULMAX, myrandom( seed++ ) ),
                                  1.0 ); // composantes de couleur
        }

        // collision avec le sol ?
        //ColorMod = ... // composantes de couleur
        // hauteur minimale à laquelle une collision avec le plancher survient
        const float hauteurPlancher = 3.0;
        
        if ( VertexMod.z <= hauteurPlancher ) {
            VertexMod.z = hauteurPlancher;
            vitesseMod.z = reflect ( vitesseMod.z, 1.0 ) / 2;
        }

        // appliquer la gravité
        vitesseMod.z = vitesseMod.z - dt * gravite;
    }
}
