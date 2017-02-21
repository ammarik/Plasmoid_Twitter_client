#ifndef MENTION_H
#define MENTION_H 

#include "tag.h" 
#include <string>
#include <iostream>

using namespace std;
/// Represtents mention in text.
class Mention : public Tag
{
public:
	Mention(int start, int end, unsigned long long id, const string& name, const string& screen_name);

	virtual int wrapTag(QString& str, int offset);
	
private:
	unsigned long long m_id;
	QString m_name;
	QString m_screen_name;

};

#endif