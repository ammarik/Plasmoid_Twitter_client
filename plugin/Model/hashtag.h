#ifndef HASHTAG_H
#define HASHTAG_H

#include "tag.h"

using namespace std;
/// Represtents hashtag in text.
class Hashtag : public Tag
{
public:
	Hashtag(int start, int end, const string& text);

	virtual int wrapTag(QString& str, int offset);

	
private:
	QString m_text;

};

#endif