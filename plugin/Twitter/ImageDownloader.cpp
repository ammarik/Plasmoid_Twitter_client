#include "ImageDownloader.h"

ImageDownloader::ImageDownloader()
{
    m_imagesPath = Config::instance().getImagesPath().toUtf8().constData();;
    m_imagePrefix = Config::instance().getImageNamePrefix().toUtf8().constData();;
} 

//------------------------------------------------------------------------------//

///Cleans images folder.
void ImageDownloader::clean()
{ 
    QString path = QString::fromStdString(m_imagesPath);
    QString pattern = QString::fromStdString(m_imagePrefix);
    pattern += "*"; 
    QDir dir(path);
    dir.setNameFilters(QStringList() << pattern);
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}

//------------------------------------------------------------------------------//

///Stars downloading of image.
/**
 * @param fileName  - name of the output image file
 * @param imageUrl  - url from where the image is downloaded.
 * @param overwrite - if false then - if exists file with same name as in param then file will not be downloaded.
 * @param overwrite - old files will be overwriten.
 */
void ImageDownloader::startDownload(const string & fileName, const string & imageUrl, bool overwrite)
{
	string file = m_imagesPath;
    file += m_imagePrefix;
    file += fileName;
        
    if(!overwrite)
    {
        if(existsFile(file))
            return;
    }

    FILE* fp = fopen(file.c_str(), "wb");
    if (!fp)
    {
        throw MyFileException(file);
        return;
    }

    CURL* curlCtx = curl_easy_init();
    curl_easy_setopt(curlCtx, CURLOPT_URL, imageUrl.c_str());
    curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
    curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

    CURLcode rc = curl_easy_perform(curlCtx);
    if (rc)
    {
        qCritical() << "Failed to download: " <<  QString::fromStdString(imageUrl);
        return;
    }

    long res_code = 0;
    curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
    if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
    {
        qCritical() << "Response code: " <<  res_code;
        return;
    }

    curl_easy_cleanup(curlCtx);

    fclose(fp);


}

//------------------------------------------------------------------------------//

///Returns information if the given file exists.
/**
 * @param  name - path of the file.
 * @return true - if the file given by path in argument exists.
 * @return false - if the file given by path in argument not exists.
 */
bool ImageDownloader::existsFile (const std::string& name) 
{
    if (FILE *file = fopen(name.c_str(), "r")) 
    {
        fclose(file);
        return true;
    }
    else 
    {
        return false;
    }   
}

//------------------------------------------------------------------------------//

///Downloaders callbask function
/**
 * Will be called when image is downloaded.
 * @param  ptr      
 * @param  size     
 * @param  nmemb    
 * @param  userdata 
 * @return          
 */
size_t ImageDownloader::callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        qCritical() << "!!! No stream\n";
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}