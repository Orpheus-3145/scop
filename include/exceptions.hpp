#pragma once
#include <string>


class ParsingException : public std::exception
{
	private:
        std::string _info;

	public:
		ParsingException( std::string const& info ) noexcept : std::exception(), _info(info) {};
		virtual ~ParsingException( void ) noexcept {};

		virtual const char* what( void ) const noexcept override {return (this->_info.c_str());};
};

class WindowException : public std::exception
{
	private:
        std::string _info;

	public:
		WindowException( std::string const& info ) noexcept : std::exception(), _info(info) {};
		virtual ~WindowException( void ) noexcept {};

		virtual const char* what( void ) const noexcept override {return (this->_info.c_str());};
};
