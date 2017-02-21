 #include "qmlBackendFactory.h"

int QmlBackendFactory::factoryInstanceCounter = 0;

/// Creates QmlBackendFactory instace and incresce static factoryInstanceCounter by 1.
QmlBackendFactory::QmlBackendFactory(QObject *parent) : QObject(parent) 
{	
	factoryInstanceCounter++;
}

//------------------------------------------------------------------------------//

/// Decrease static factoryInstanceCounter and if couter is less than one, then destroys Backend. Destroys itself.
QmlBackendFactory::~QmlBackendFactory()
{
	factoryInstanceCounter--;
	if(factoryInstanceCounter < 1)
		Backend::destroy();
}

//------------------------------------------------------------------------------//

/// Return Backend instance
/**
 * @return instace of Backend
 */
QObject* QmlBackendFactory::getBackend() const
{
	return Backend::instance();
}