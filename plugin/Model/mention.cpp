#include "mention.h"

/// Creates new mention
/**
 * @param start - start position of the mention in text
 * @param end - end position of the mention in text
 * @param id - id of the mentioned user
 * @param name - name of the mentioned user
 * @param screen_name - mentioned screen name 
 */
Mention::Mention(int start, int end, unsigned long long id, const string& name, const string& screen_name)
 : Tag(start, end), m_id(id)
{
	m_name = QString::fromStdString(name);
	m_screen_name = QString::fromStdString(screen_name);
}

//------------------------------------------------------------------------------//

///Wraps tag into html link.
/**
 * @param  str text
 * @param  offset of the mention.
 * @return  text with wrapped mention.
 */
int Mention::wrapTag(QString& str, int offset)
{
	QString s = str;
	QString pre = "<a href=\"https://twitter.com/";
	QString im = "\" style=\"text-decoration:none; color:blue; \">";
	QString post = "</a>";

	s.insert(m_start + offset, pre);
	offset += pre.length();

	s.insert(m_start + offset, m_screen_name);
	offset += m_screen_name.length();

	s.insert(m_start + offset, im);
	offset += im.length();

	s.insert(m_end + offset, post);
	offset += post.length();

	str = s;

	return offset;
}