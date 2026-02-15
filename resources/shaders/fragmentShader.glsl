#version 330 core

in vec3 colorRGB;
in vec2 textCoor;
out vec4 FragColor;

uniform sampler2D myTexture;
uniform float blendingLevel;

void main()
{
    vec4 texColor = texture(myTexture, textCoor);
    vec4 rgbColor = vec4(colorRGB, 1.0);
    FragColor = mix(rgbColor, texColor, blendingLevel);
}
