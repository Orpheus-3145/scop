#pragma once
#include <string>

class ParsingException : public std::exception
{
	private:
        std::string info;

	public:
		ParsingException( std::string const& info ) noexcept : std::exception(), info(info) {};
		virtual ~ParsingException( void ) noexcept {};

		virtual const char* what( void ) const noexcept override {return (this->info.c_str());};
};