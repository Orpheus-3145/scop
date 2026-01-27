#version 330 core

in vec3 colorRGB;
in vec2 textCoor;
out vec4 FragColor;

uniform sampler2D myTexture;
uniform bool applyTexture;

void main()
{
    if (applyTexture == true)
    	FragColor = texture(myTexture, textCoor);
    else
        FragColor = vec4(colorRGB, 1.0f);
}
