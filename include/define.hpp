#pragma once
#include<string>
#include <cstdint>

constexpr char const* SCOP_OBJECT_FILE = "resources/objFiles/cube.obj";

constexpr uint32_t SCOP_WINDOW_WIDTH = 1920;
constexpr uint32_t SCOP_WINDOW_HEIGHT = 1080;

constexpr char const* SCOP_VERTEX_SHADER = "resources/shaders/vertexShader.glsl";
constexpr char const* SCOP_FRAGMENT_SHADER = "resources/shaders/fragmentShader.glsl";
constexpr char const* SCOP_TEXTURE_CAPYBARA = "resources/textures/capybara.jpg";

constexpr bool SCOP_COLUMN_MAJOR = true;
constexpr float SCOP_CAMERA_SPEED = 0.75;
constexpr float	SCOP_FOV = 45.0f;
constexpr float	SCOP_NEAR = 1.0f;
constexpr float	SCOP_FAR = 100.0f;
constexpr bool	SCOP_PRJ_FINITE = true;