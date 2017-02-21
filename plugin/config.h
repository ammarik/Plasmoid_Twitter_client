#ifndef CONFIG_H
#define CONFIG_H 

#include <cstdlib>
#include <iostream>
#include <QString>
#include <QStandardPaths>

using namespace std;

/// Globlal configuration properties of the Backend.(Singleton)
class Config
{
	public:
		static Config& instance();
		unsigned long long getSignedUserId() const;
		void setSignedUserId(unsigned long long id);
		QString getFilesPath();
		QString getImagesPath();
		QString getImageNamePrefix();

	private:
		Config();
		Config(const Config & c);
		Config & operator=(const Config & c);

		unsigned long long m_signedUserId;

		static const QString FILES_PATH;
		QString m_filesPath;
		static const QString IMAGES_PATH;
		QString m_imagesPath;
		static const QString IMAGE_NAME_PREFIX;
};

#endif