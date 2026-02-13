#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;
layout (location = 2) in vec2 aNormCoord;
layout (location = 3) in vec3 aColorRGB;

out vec3 colorRGB;
out vec2 textCoor;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec4 posArray = vec4(aPos, 1.0);
	gl_Position = projection * view * model * posArray;
	colorRGB = aColorRGB;
	textCoor = aTextCoord;
    vec4 worldPos = model * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
}
