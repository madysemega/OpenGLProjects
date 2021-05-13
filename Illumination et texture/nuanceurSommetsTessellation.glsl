#version 410

uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;
layout(location=8) in vec4 TexCoord;


out Attribs {
    vec3 normale;
    vec2 texCoord;
    vec4 Vertex;
} AttribsOut;

uniform float temps;

float attenuation = 1.0;

void main( void ) 
{
    AttribsOut.Vertex = Vertex;
    AttribsOut.texCoord = TexCoord.st - vec2(0.1 * temps, 0);
    AttribsOut.normale = matrNormale * Normal;
}