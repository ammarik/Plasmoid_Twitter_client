#ifndef URL_H
#define URL_H 

#include "tag.h" 
#include <string>

using namespace std;
/// Represtents Url in text.
class Url : public Tag
{
public:
	Url(int start, int end, const string& display_url, const string& expanded_url, const string& url);

	virtual int wrapTag(QString& str, int offset);
	
private:
	QString m_display_url;
	QString m_expanded_url;
	QString m_url;

};

#endif