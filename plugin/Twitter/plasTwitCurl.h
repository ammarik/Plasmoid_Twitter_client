#ifndef PLASTWITCURL_H
#define PLASTWITCURL_H

#include "libtwitcurl/twitcurl.h"

/// Inherits from twitCurl and adding possibility to get curl object.
class PlasTwitCurl : public twitCurl
{
public:
    CURL* getCurl() {return m_curlHandle;}
private:
};

#endif // PLASTWITCURL_H
