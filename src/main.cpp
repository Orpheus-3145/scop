#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <optional>
#include <functional>

#include "exceptions.hpp"
#include "define.hpp"
#include "scopGL/scopGL.hpp"


static constexpr const char* HOW_TO = R"(Usage: ./scop [options]
Program that renders an obj file (see for reference: https://en.wikipedia.org/wiki/Wavefront_.obj_file)
Directives supported: v, vt, vn, f. Materials and lights not implemented yet.

	-f,  --file             object file (e.g. generated from Blender) to render
	-w,  --width            width in pixel of the window
	-h,  --height           height in pixel of the window
	-vs, --vertexShader     file for shader
	-ts, --textureShade     file for shader
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

	static void setObjectFile( InputData& input, std::optional<std::string> optValue ) {
		if (optValue.has_value() == false)
			throw ParsingException("Missing value for --file/-f");
		input.objFile = optValue.value();
	}

	static void setWidth( InputData& input, std::optional<std::string> optValue ) {
		try {
			input.width = std::stoi(optValue.value());
		} catch (std::bad_optional_access const&) {
			throw ParsingException("Missing value for --width/-w");
		} catch (std::invalid_argument const&) {
			throw ParsingException("Wrong number input: " + optValue.value());
		} catch (std::out_of_range const&) {
			throw ParsingException("Out of range: " + optValue.value());
		}
	}

	static void setHeight( InputData& input, std::optional<std::string> optValue ) {
		try {
			input.height = std::stoi(optValue.value());
		} catch (std::bad_optional_access const&) {
			throw ParsingException("Missing value for --height/-h");
		} catch (std::invalid_argument const&) {
			throw ParsingException("Wrong number input: " + optValue.value());
		} catch (std::out_of_range const&) {
			throw ParsingException("Out of range: " + optValue.value());
		}
	}

	static void setVertexShaderOpt( InputData& input, std::optional<std::string> optValue ) {
		if (optValue.has_value() == false)
			throw ParsingException("Missing value for --vertexShader/-vs");
		input.vertexShaderFile = optValue.value();
		
	}

	static void setFragmentShaderOpt( InputData& input, std::optional<std::string> optValue ) {
		if (optValue.has_value() == false)
			throw ParsingException("Missing value for --fragmentShader/-fs");
		input.fragmentShaderFile = optValue.value();
		
	}

	static void setTextureFile( InputData& input, std::optional<std::string> optValue ) {
		if (optValue.has_value() == false)
			throw ParsingException("Missing value for --texture/-t");
		input.textureFile = optValue.value();
	}

	static void setHelpMode( InputData& input, std::optional<std::string> optValue ) {
		(void)optValue;
		input.helpmode = true;
	}
};

enum class OptionType { ObjectFile, Width, Height, VertexShaderFile, TextureShaderFile, TextureFile, Helpmode };

static const std::map<std::string, OptionType> flagMap = {
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

static const std::map<OptionType, std::function<void( InputData&, std::optional<std::string> )>> flagActions = {
	{OptionType::ObjectFile, InputData::setObjectFile},
	{OptionType::Width, InputData::setWidth},
	{OptionType::Height, InputData::setHeight},
	{OptionType::VertexShaderFile, InputData::setVertexShaderOpt},
	{OptionType::TextureShaderFile, InputData::setFragmentShaderOpt},
	{OptionType::TextureFile, InputData::setTextureFile},
	{OptionType::Helpmode, InputData::setHelpMode},
};

InputData parseArgs(int32_t argc, char* argv[]) {
    InputData opts;
	std::string arg, value;
	OptionType option;

    for (int32_t i = 1; i < argc; ++i) {
		arg = argv[i];

		// support for: argoment=value
        size_t eqPos = arg.find('=');
        if (eqPos != std::string::npos) {		// if it is: --key=value
			if (eqPos == 0 or eqPos == arg.size() - 1)
				throw ParsingException("invalid argument with value: " + arg);
            value = arg.substr(eqPos + 1);
            arg = arg.substr(0, eqPos);
        } else if ((i + 1 < argc) and (argv[i + 1][0] != '-'))		// or is: --key value
            value = argv[++i];

        auto mapOption = flagMap.find(arg);
        if (mapOption == flagMap.end())
            throw ParsingException("Unknown argument: " + arg);
		option = mapOption->second;

		auto it = flagActions.find(option);
        if (it != flagActions.end())
            it->second(opts, value);	// run action depending on option type
	}
    return opts;
}

int32_t main(int32_t argc, char** argv) {
	try {
		InputData options = parseArgs(argc, argv);

		if (options.helpmode == true) {
			std::cout << HOW_TO << std::endl;
		} else {
			ScopGL app = ScopGL();
		
			app.parseFile(options.objFile);
			app.createWindow(options.width, options.height);
			app.initGL(options.vertexShaderFile, options.fragmentShaderFile, options.textureFile);
			app.sendBuffersToGPU();
			app.start();
		}
	} catch (AppException& err) {
		std::cerr << err.what() << std::endl;
		return(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}