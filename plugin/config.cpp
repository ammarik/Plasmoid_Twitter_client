#include <stddef.h>  // defines NULL
#include "config.h"

//------------------------------------------------------------------------------// 

const QString Config::FILES_PATH = "/plasmoids/cz.cvut.fit.marikma5/";
const QString Config::IMAGES_PATH = "/plasmoids/cz.cvut.fit.marikma5/img/";
const QString Config::IMAGE_NAME_PREFIX = "image_";

//------------------------------------------------------------------------------//

Config::Config()
{
	m_signedUserId = 0;

	QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	m_filesPath = writableLocation;
	m_filesPath += FILES_PATH;
	m_imagesPath = writableLocation;
	m_imagesPath += IMAGES_PATH;
}

//------------------------------------------------------------------------------//

/// Returns single instance of Config.
Config& Config::instance()
{
	static Config instance; // Guaranteed to be destroyed.
                            // Instantiated on first use.
	return instance;
}

//------------------------------------------------------------------------------//


/// Returns the ID of the logged-in user.
/**
 * @return id of the logged-in user.
 */
unsigned long long Config::getSignedUserId() const
{
	return m_signedUserId;
}

//------------------------------------------------------------------------------//

/// Set the ID of the logged-in user.
/**
 * @param id of the logged-in user.
 */
void Config::setSignedUserId(unsigned long long id)
{
	m_signedUserId = id;
}

//------------------------------------------------------------------------------//

/// Returns the path for saving files. 
/**
 * @return path for saving files.
 */
QString Config::getFilesPath()
{
	return m_filesPath;
}

//------------------------------------------------------------------------------//

/// Returns the path for saving images.
/**
 * @return path for saving images.
 */
QString Config::getImagesPath()
{
	return m_imagesPath;
}

//------------------------------------------------------------------------------//

/// Returns prefix, with which the image titles should start.
/**
 * @return image title prefix.
 */
QString Config::getImageNamePrefix()
{
	return IMAGE_NAME_PREFIX;
}

