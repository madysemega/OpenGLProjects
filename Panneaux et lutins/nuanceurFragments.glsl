#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec4 couleur;
    vec2 texCoords;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    vec4 tex = texture( leLutin, AttribsIn.texCoords );
    FragColor = AttribsIn.couleur;
    
    if ( texnumero > 0 ) {
        if (tex.a < 0.1) {
            discard;
        }

        FragColor = vec4( mix( FragColor.rgb, tex.rgb, 0.6 ), AttribsIn.couleur.a );
    }
}
