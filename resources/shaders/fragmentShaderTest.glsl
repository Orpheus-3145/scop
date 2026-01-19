#version 330 core

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	// FragColor = texture(myTexture, textCoor);
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
