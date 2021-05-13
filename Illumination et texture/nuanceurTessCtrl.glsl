#version 410

layout(vertices = 4) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

in Attribs {
	vec3 normale;
    vec2 texCoord;
    vec4 Vertex;
} AttribsIn[];

out Attribs {
	vec3 normale;
    vec2 texCoord;
    vec4 Vertex;
} AttribsOut[];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	if (gl_InvocationID == 0)
	{
		for (int i = 0; i < 2; i++){
			gl_TessLevelInner[i] = TessLevelInner;
		}
		for (int i = 0; i < 4; i++) {
			gl_TessLevelOuter[i] = TessLevelOuter;
		}
	}

	AttribsOut[gl_InvocationID].normale = AttribsIn[gl_InvocationID].normale;
	AttribsOut[gl_InvocationID].texCoord = AttribsIn[gl_InvocationID].texCoord;
	AttribsOut[gl_InvocationID].Vertex = AttribsIn[gl_InvocationID].Vertex;
}