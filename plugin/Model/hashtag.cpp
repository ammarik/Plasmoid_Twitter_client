#include "hashtag.h"

/// Creates new hashtag
/**
 * @param start position of the hashtag in text
 * @param end position of the hashtag in text
 * @param text
 */
Hashtag::Hashtag(int start, int end, const string& text) : Tag(start, end)
{
	m_text = QString::fromStdString(text);
}

//------------------------------------------------------------------------------//

///Wraps tag into html link.
/**
 * @param  str text
 * @param  offset of the hashtag.
 * @return  text with wrapped hashtag.
 */
int Hashtag::wrapTag(QString& str, int offset)
{
	QString s = str;
	QString pre = "<a href=\"https://twitter.com/hashtag/";
	QString im = "?src=hash\" style=\"text-decoration:none; color:blue;\">";
	QString post = "</a>";

	s.insert(m_start + offset, pre);
	offset += pre.length();

	s.insert(m_start + offset, m_text);
	offset += m_text.length();

	s.insert(m_start + offset, im);
	offset += im.length();

	s.insert(m_end + offset, post);
	offset += post.length();

	str = s;

	return offset;
}