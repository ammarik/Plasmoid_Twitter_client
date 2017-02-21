#ifndef IMAGEDOWNLOADER_H
#define	IMAGEDOWNLOADER_H

#include <curl/curl.h>
#include <string>
#include <stdio.h>
#include <cstdlib>

#include <QString>
#include <QDir>
#include <QDebug>

#include "../config.h"
#include "../Exceptions/permFileException.h"
#include "../Exceptions/myFileException.h"

using namespace std;

///Downloads images from server
class ImageDownloader
{
public:
	ImageDownloader();

	void clean();
	void startDownload(const string & fileName, const string & imageUrl, bool overwrite);

private:
	string m_imagesPath;
	string m_imagePrefix;

	bool existsFile (const std::string& name); 
	static size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata);

};

#endif
