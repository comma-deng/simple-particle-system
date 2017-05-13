#version 330 core
layout (points) in;
layout (points, max_vertices = 6) out;

in VS_OUT {
    vec4 color;
	vec3 dir;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fColor;

void main() 
{
	fColor = gs_in[0].color;
	
	mat4 mvp = projection * view *  model;

	for (int i=0;i<6;i++)
	{
		gl_Position = mvp * (gl_in[0].gl_Position - i * vec4(gs_in[0].dir,0.0) /10);
		EmitVertex();
	}
	
	//gl_Position = mvp * (gl_in[0].gl_Position );

	//EmitVertex();
    EndPrimitive();
}