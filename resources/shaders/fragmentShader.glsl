#version 330 core

in vec3 colorRGB;
in vec2 textCoor;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	FragColor = texture(myTexture, textCoor);
    //FragColor = vec4(colorRGB, 1.0f);
}
