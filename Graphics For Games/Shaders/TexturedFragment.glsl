#version 150 core
uniform sampler2D diffuseTex ;

in Vertex {
	vec2 texCoord ;
	vec4 pos;
} IN ;

out vec4 fragColour ;

void main ( void ) {
	fragColour = texture (diffuseTex, IN.texCoord); // vec4(vec2(IN.texCoord),0, 1);
}