#include "argParser.hpp"
#include "exception.hpp"


void InputData::setObjectFile( InputData& input, std::optional<std::string> optValue ) {
    if (optValue.has_value() == false)
        throw ParsingException("Missing value for --file/-f");
    input.objFile = optValue.value();
}

void InputData::setWidth( InputData& input, std::optional<std::string> optValue ) {
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

void InputData::setHeight( InputData& input, std::optional<std::string> optValue ) {
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

void InputData::setVertexShaderOpt( InputData& input, std::optional<std::string> optValue ) {
    if (optValue.has_value() == false)
        throw ParsingException("Missing value for --vertexShader/-vs");
    input.vertexShaderFile = optValue.value();
    
}

void InputData::setFragmentShaderOpt( InputData& input, std::optional<std::string> optValue ) {
    if (optValue.has_value() == false)
        throw ParsingException("Missing value for --fragmentShader/-fs");
    input.fragmentShaderFile = optValue.value();
    
}

void InputData::setTextureFile( InputData& input, std::optional<std::string> optValue ) {
    if (optValue.has_value() == false)
        throw ParsingException("Missing value for --texture/-t");
    input.textureFile = optValue.value();
}

void InputData::setHelpMode( InputData& input, std::optional<std::string> optValue ) {
    (void)optValue;
    input.helpmode = true;
}


InputData InputData::parseArgs(int32_t argc, char* argv[]) {
    InputData opts;
	std::string arg, value;
	OptionType option;

    for (int32_t i = 1; i < argc; ++i) {
		arg = argv[i];

		// support for: argoment=value
        size_t eqPos = arg.find('=');
        if (eqPos != std::string::npos) {		// if it is: --key=value
			if (eqPos == 0 or eqPos == arg.size() - 1)
				throw ParsingException("Invalid argument with value: " + arg);
            value = arg.substr(eqPos + 1);
            arg = arg.substr(0, eqPos);
        } else if ((i + 1 < argc) and (argv[i + 1][0] != '-'))		// or is: --key value
            value = argv[++i];

        auto mapOption = flagMap.find(arg);
        if (mapOption == flagMap.cend())
            throw ParsingException("Unknown argument: " + arg);
		option = mapOption->second;

		auto it = flagActions.find(option);
        if (it != flagActions.cend())
            it->second(opts, value);	// run action depending on option type
	}
    return opts;
}
