#version 330 core
	
out vec4 FragColor;

in vec3 newColor;
in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D ourTexture;

void main()
{
	//FragColor = vec4(newColor, 1.0);
	//FragColor = ourColor; 
	FragColor = texture(ourTexture, TexCoord) * vec4(newColor, 1);
}
