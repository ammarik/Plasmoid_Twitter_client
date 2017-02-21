#ifndef TAG_H
#define TAG_H 

#include <QString>
#include <QDebug>

using namespace std;
///Abstract tag in text.
class Tag
{
public:
	Tag(int start, int end);
	virtual ~Tag();

	int getStartPosition() const;
	int getEndPosition() const;
	int getLength() const;
	virtual int wrapTag(QString& str, int offset) = 0;

	bool operator < (const Tag &t) const;
protected:
	int m_start;
	int m_end;

};

#endif