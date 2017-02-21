#include "url.h"

/// Creates new Url
/**
 * @param start - start position of the url in text
 * @param end - end position of the url in text
 * @param display_url
 * @param expanded_url 
 * @param url
 */
Url::Url(int start, int end, const string& display_url, const string& expanded_url, const string& url)
 : Tag(start, end)
{
	m_display_url = QString::fromStdString(display_url); 
	m_expanded_url = QString::fromStdString(expanded_url);
	m_url = QString::fromStdString(url);
}

//------------------------------------------------------------------------------//

///Wraps tag into html link.
/**
 * @param  str text
 * @param  offset of the media.
 * @return  text with wrapped media.
 */
int Url::wrapTag(QString& str, int offset)
{
	QString s = str;
	QString pre = "<a href=\"";
	QString im = "\" style=\"text-decoration:none; color:blue;\">";
	QString post = "</a>";

	QString wrappedLink = pre;
	wrappedLink += m_expanded_url;
	wrappedLink += im;
	wrappedLink += m_display_url;
	wrappedLink += post;

	int lengthOfOriginalLink = m_end - m_start;
	int lengthOfNewLink = wrappedLink.length();

	s.replace(m_start + offset, lengthOfOriginalLink, wrappedLink);

	offset += lengthOfNewLink - lengthOfOriginalLink;

	str = s;

	return offset;
}