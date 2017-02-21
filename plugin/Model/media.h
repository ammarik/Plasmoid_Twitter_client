#ifndef MEDIA_H
#define MEDIA_H 

#include "tag.h" 
#include <string>

using namespace std;
/// Represtents media in text.
class Media : public Tag
{
public:
	Media(
		int start,
	 	int end,
	 	const string& display_url,
	 	const string& expanded_url, 
	 	const string& media_url,
	 	const string& media_url_https,
	 	unsigned long long id);

	virtual int wrapTag(QString& str, int offset);

	
private:
	QString m_display_url;
	QString m_expanded_url;
	QString m_media_url;
	QString m_media_url_https;
	unsigned long long m_id;

};

#endif