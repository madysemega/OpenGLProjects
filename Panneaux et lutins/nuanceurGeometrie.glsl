#version 410

layout(points) in;
// layout(points, max_vertices = 1) out;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matrProj;

uniform int texnumero;
uniform float tempsDeVieMax;

in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    //float sens; // du vol (partie 3)
    //float hauteur; // du vol (partie 3)
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec2 texCoords;
} AttribsOut;

// la hauteur minimale en-dessous de laquelle les lutins ne tournent plus (partie 3)
const float hauteurInerte = 8.0;

void main()
{
    // construire une matrice de rotation
    mat2 matrRotation = mat2( cos( 6*AttribsIn[0].tempsDeVieRestant ), -sin( 6*AttribsIn[0].tempsDeVieRestant ),
                              sin( 6*AttribsIn[0].tempsDeVieRestant ),  cos( 6*AttribsIn[0].tempsDeVieRestant ) );

    vec2 coins[4];
    coins[0] = vec2( -0.5,  0.5 );
    coins[1] = vec2( -0.5, -0.5 );
    coins[2] = vec2(  0.5,  0.5 );
    coins[3] = vec2(  0.5, -0.5 );

    for ( int i = 0 ; i < 4 ; ++i )
    {
        vec2 dpixels = gl_in[0].gl_PointSize * coins[i];

        if ( texnumero == 1 ) {
            dpixels *=  matrRotation;
		}

        vec4 pos = vec4( gl_in[0].gl_Position.xy + dpixels,
                         gl_in[0].gl_Position.zw );
        
        // assigner la position du point
        gl_Position = matrProj * pos;

        // assigner la taille des points (en pixels)
        gl_PointSize = gl_in[0].gl_PointSize;

        // assigner la couleur courante
        AttribsOut.couleur = AttribsIn[0].couleur;

        // définir des coordonnées de texture
        vec2 texCoord = coins[i] + vec2( 0.5, 0.5 );

        if (texnumero == 2) {
            const float nlutins = 12.0;
            int num = int ( mod ( 18.0 * AttribsIn[0].tempsDeVieRestant , nlutins ) );
            texCoord.x = ( texCoord.x + num ) / nlutins;        
        }

        AttribsOut.texCoords = texCoord;

        EmitVertex();
    }
}
