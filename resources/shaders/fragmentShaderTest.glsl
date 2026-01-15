#version 330 core

in vec2 textCoor;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	//FragColor = texture(myTexture, textCoor);
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
