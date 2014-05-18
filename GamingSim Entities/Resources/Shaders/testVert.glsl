#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;



in  vec3 position;
in  vec2 texCoord;
in  vec4 colour;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
	float depth;
} OUT;



void main(void)	{
	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	
	OUT.texCoord	= texCoord;
	OUT.colour		= colour;
	OUT.depth		= gl_Position.z;
	
	//OUT.colour = colour;
}