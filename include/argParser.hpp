#pragma once
#include <map>
#include <cstdint>
#include <optional>
#include <functional>
#include <string>

#include "define.hpp"


constexpr const char* HOW_TO = R"(Usage: ./scop [options]
Program that renders an obj file (see for reference: https://en.wikipedia.org/wiki/Wavefront_.obj_file)
Directives supported: v, vt, vn, f. Materials and lights not implemented yet.

	-f,  --file             object file (e.g. generated from Blender) to render
	-w,  --width            width in pixel of the window
	-h,  --height           height in pixel of the window
	-vs, --vertexShader     file for vertex shader
	-ts, --textureShader     file for fragment shader
	-t,  --texture          texture file to apply to the object
	     --help             print info

	[options can be set with next word or = : --opt value  | --opt=value ]
)";

struct InputData {
	std::string objFile = SCOP_OBJECT_FILE;
	uint32_t	width = SCOP_WINDOW_WIDTH;
	uint32_t	height = SCOP_WINDOW_HEIGHT;
	std::string vertexShaderFile = SCOP_VERTEX_SHADER;
	std::string fragmentShaderFile = SCOP_FRAGMENT_SHADER;
	std::string textureFile = SCOP_TEXTURE_CAPYBARA;
	bool		helpmode = false;

    static InputData    parseArgs( int32_t, char** ) ;
	static void         setObjectFile( InputData&, std::optional<std::string> );
	static void         setWidth( InputData&, std::optional<std::string> );
	static void         setHeight( InputData&, std::optional<std::string> );
	static void         setVertexShaderOpt( InputData&, std::optional<std::string> );
	static void         setFragmentShaderOpt( InputData&, std::optional<std::string> );
	static void         setTextureFile( InputData&, std::optional<std::string> );
	static void         setHelpMode( InputData&, std::optional<std::string> );
};

enum class OptionType {
    ObjectFile,
    Width,
    Height,
    VertexShaderFile,
    TextureShaderFile,
    TextureFile,
    Helpmode
};

const std::map<std::string, OptionType> flagMap = {
	{"-f", OptionType::ObjectFile},
	{"--file", OptionType::ObjectFile},
	{"-w", OptionType::Width},
	{"--width", OptionType::Width},
	{"-h", OptionType::Height},
	{"--height", OptionType::Height},
	{"-vs", OptionType::VertexShaderFile},
	{"--vertexShader", OptionType::VertexShaderFile},
	{"-ts", OptionType::TextureShaderFile},
	{"--textureShader", OptionType::TextureShaderFile},
	{"-t", OptionType::TextureFile},
	{"--texture", OptionType::TextureFile},
	{"--help", OptionType::Helpmode}
};

const std::map<OptionType, std::function<void( InputData&, std::optional<std::string> )>> flagActions = {
	{OptionType::ObjectFile, InputData::setObjectFile},
	{OptionType::Width, InputData::setWidth},
	{OptionType::Height, InputData::setHeight},
	{OptionType::VertexShaderFile, InputData::setVertexShaderOpt},
	{OptionType::TextureShaderFile, InputData::setFragmentShaderOpt},
	{OptionType::TextureFile, InputData::setTextureFile},
	{OptionType::Helpmode, InputData::setHelpMode},
};
