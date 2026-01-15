#pragma once
#include <string>


// NB change so it inherits from runtime_error
class AppException : public std::exception
{
	private:
        std::string _info;

	public:
		AppException( std::string const& info ) noexcept : std::exception(), _info(info) {};
		virtual ~AppException( void ) noexcept {};

		virtual const char* what( void ) const noexcept override {return (this->_info.c_str());};
};

class ParsingException : public AppException {
	public:
		using AppException::AppException;
};

class GlfwException : public AppException {
	public:
		using AppException::AppException;
};

class MathException : public AppException {
	public:
		using AppException::AppException;
};

