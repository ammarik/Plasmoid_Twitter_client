#ifndef MISSINGSIGNEDUSEREXCEPT_H
#define MISSINGSIGNEDUSEREXCEPT_H 

#include <exception>
#include <string> 

using namespace std;
/// Exception - There is no user with userId same with signedUserId.
class MissingSignedUserException : public exception
{
public:
	MissingSignedUserException(unsigned long long signedUser, unsigned long long userA, unsigned long long userB);
	const char * what() const throw();
private:
	unsigned long long m_signedUser;
	unsigned long long m_userA;
	unsigned long long m_userB;
};

#endif