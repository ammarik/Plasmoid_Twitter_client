#include "tag.h"

Tag::Tag(int start, int end) : m_start(start), m_end(end)
{

}

//------------------------------------------------------------------------------//

Tag::~Tag()
{

}

//------------------------------------------------------------------------------//

/// Returns start position of tag in text.
/**
 * @return start position of tag.
 */
int Tag::getStartPosition() const
{
	return m_start;
}

//------------------------------------------------------------------------------//

///Returns end position of tag in text.
/**
 * @return end position of tag.
 */
int Tag::getEndPosition() const
{
	return m_end;
}

//------------------------------------------------------------------------------//

///Returns length of tag.
/**
 * @return length of tag.
 */
int Tag::getLength() const
{
	return m_end - m_start;
}

//------------------------------------------------------------------------------//

/// Comapres two tags by starting position.
bool Tag::operator < (const Tag &t) const
{
	return (m_start < t.getStartPosition());
}