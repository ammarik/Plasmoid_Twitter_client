#ifndef CREATEFOLDEREXCEPT_H
#define CREATEFOLDEREXCEPT_H 

#include <exception>
#include <string> 
#include <QString>

using namespace std;
/// Exception - Create folder error.
class CreateFolderException : public exception
{
public:
	CreateFolderException(const QString& path);
	const char * what() const throw();
private:
	string m_path;
};

#endif