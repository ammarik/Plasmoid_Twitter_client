#ifndef PERMFILEEXCEPT_H
#define PERMFILEEXCEPT_H 

#include <exception>
#include <string> 

using namespace std;
/// Exception - Set file permissions error.
class PermFileException : public exception
{
public:
	PermFileException(const string& path);
	const char * what() const throw();
private:
	string m_path;
};

#endif