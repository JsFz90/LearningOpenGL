#version 330 core
	
out vec4 FragColor;

in vec3 newColor;
in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = vec4(newColor, 1.0);
	//FragColor = ourColor; 
	//FragColor = texture(ourTexture, TexCoord) * vec4(newColor, 1);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);   //80% color 1ra textura - 20% color 2da textura
}
