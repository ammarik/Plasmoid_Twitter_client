#ifndef MYFILEEXCEPT_H
#define MYFILEEXCEPT_H 

#include <exception>
#include <string> 

using namespace std;
/// Exception - Create file error.
class MyFileException : public exception
{
public:
	MyFileException(const string& path);
	const char * what() const throw();
private:
	string m_path;
};

#endif