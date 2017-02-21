#include "permFileException.h"


PermFileException::PermFileException(const string& path) : m_path(path)
{
}

//------------------------------------------------------------------------------//

const char * PermFileException::what() const throw()
{
	string s = "Error - Unable to se permissions of file: \""  + m_path + "\".";
	return s.c_str();
}