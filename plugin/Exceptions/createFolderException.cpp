#include "createFolderException.h"


CreateFolderException::CreateFolderException(const QString& path)
{
	m_path = path.toUtf8().constData();
}

//------------------------------------------------------------------------------//

const char * CreateFolderException::what() const throw()
{
	string s = "Cannot create folder in path: \""  + m_path + "\".";
	return s.c_str();
}