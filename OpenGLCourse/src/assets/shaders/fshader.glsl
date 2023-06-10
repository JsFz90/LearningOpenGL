#version 330 core
	
out vec4 FragColor;
in vec3 newColor;
uniform vec4 ourColor;

void main()
{
	//FragColor = vec4(newColor, 1.0);
	FragColor = ourColor; 
}
