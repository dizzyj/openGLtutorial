#version 330 core
//inputs
in vec3 color;
in vec2 texCoord;
//outputs
out vec4 FragColor;
//uniforms
uniform vec3 colorOffset;
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}