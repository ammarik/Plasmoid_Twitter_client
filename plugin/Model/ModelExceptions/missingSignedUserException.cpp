#include "missingSignedUserException.h"


MissingSignedUserException::MissingSignedUserException(
	unsigned long long signedUser,
	unsigned long long userA,
	unsigned long long userB) : m_signedUser(signedUser), m_userA(userA), m_userB(userB)
{

}

//------------------------------------------------------------------------------//
//
const char * MissingSignedUserException::what() const throw()
{
	string s = "There is no user with userId same with signedUserId. userA ID: " + to_string(m_userA) +
				", userB ID: " + to_string(m_userB) +
				", signed user ID: " + to_string(m_signedUser); 
	return s.c_str();
}