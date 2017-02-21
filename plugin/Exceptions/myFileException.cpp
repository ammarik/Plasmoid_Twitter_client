#include "myFileException.h"


MyFileException::MyFileException(const string& path) : m_path(path)
{
}

//------------------------------------------------------------------------------//

const char * MyFileException::what() const throw()
{
	string s = "Cannot create file in path: \""  + m_path + "\".";
	return s.c_str();
}