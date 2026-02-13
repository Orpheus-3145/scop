#version 330 core

in vec3 colorRGB;
in vec2 textCoor;
in vec3 fragPos;
out vec4 FragColor;

uniform float u_time;
uniform sampler2D myTexture;
uniform bool applyTexture;

void main()
{
    // if (u_time >= 2.0)
    // 	FragColor = texture(myTexture, textCoor);
    // else
    //     FragColor = vec4(colorRGB, 1.0f);

    float progress = clamp(u_time / 1.0, 0.0, 1.0);

    float mask = step(progress, fragPos.x);

    vec3 texColor = texture(myTexture, textCoor).rgb;

    vec3 finalColor = mix(texColor, colorRGB, mask);

    FragColor = vec4(finalColor, 1.0);
}
