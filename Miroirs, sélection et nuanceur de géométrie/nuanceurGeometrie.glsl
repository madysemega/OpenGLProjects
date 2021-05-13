#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Attribs {
	vec4 couleur;
} AttribsIn[];

out Attribs {
	vec4 couleur;
	vec3 lumiDir, normale, obsVec;
	
} AttribsOut;

void main( void )
{
	AttribsOut.lumiDir = vec3(0, 0, 1);
	AttribsOut.obsVec = vec3(0, 0, 1);
	vec3 sommet0 = gl_in[0].gl_Position.xyz;
	vec3 sommet1 = gl_in[1].gl_Position.xyz;
	vec3 sommet2 = gl_in[2].gl_Position.xyz;
	vec3 arete1 = (sommet1 - sommet0);
	vec3 arete2 = (sommet2 - sommet0);
	AttribsOut.normale = cross (arete1, arete2);
	for(int i = 0; i < gl_in.length(); ++i){
		gl_Position = gl_in[i].gl_Position;
		AttribsOut.couleur = AttribsIn[i].couleur;
		EmitVertex();
	}
	EndPrimitive();
}