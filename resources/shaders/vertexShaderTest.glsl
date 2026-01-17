#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColorRGB;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec2 aNormCoord;

out vec3 colorRGB;
out vec2 textCoor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec4 posArray = vec4(aPos, 1.0);
	gl_Position = projection * view * model * posArray;
	colorRGB = aColorRGB;
	textCoor = aTextCoord;
}