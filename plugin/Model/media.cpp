#include "media.h"

/// Creates new media
/**
 * @param start position of the media in text
 * @param end position of the media in text
 * @param display_url - viz Twitter documentation
 * @param expanded_url - viz Twitter documentation
 * @param media_url - viz Twitter documentation
 * @param media_url_https - viz Twitter documentation
 * @param id
 */
Media::Media(
		int start,
	 	int end,
	 	const string& display_url,
	 	const string& expanded_url, 
	 	const string& media_url,
	 	const string& media_url_https,
	 	unsigned long long id) : Tag(start, end)
{
	m_display_url = QString::fromStdString(display_url);
	m_expanded_url = QString::fromStdString(expanded_url); 
	m_media_url = QString::fromStdString(media_url);
	m_media_url_https = QString::fromStdString(media_url_https);
	m_id = id;
}

//------------------------------------------------------------------------------//

///Wraps tag into html link.
/**
 * @param  str text
 * @param  offset of the media.
 * @return  text with wrapped media.
 */
int Media::wrapTag(QString& str, int offset)
{
	QString s = str;

	QString pre = "<p>Media: <a href=\"";
	QString im = "\" style=\"text-decoration:none; color:blue;\">";
	QString post = "</a></p>";

	QString wrappedLink = pre;
	wrappedLink += m_media_url;
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