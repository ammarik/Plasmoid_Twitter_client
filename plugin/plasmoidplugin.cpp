#include "plasmoidplugin.h"

/// Registers C++ classes as a new QML types.
/**
 * @param uri 
 */
void PlasmoidPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("cz.cvut.fit.twitterPlasmoid"));
    
	qmlRegisterType<QmlBackendFactory>(uri, 1, 0, "BackendFactory");
    qmlRegisterUncreatableType<StatusesModel>(uri, 1, 0, "StatusesModel", "");
    qmlRegisterUncreatableType<NotificatonsModel>(uri, 1, 0, "NotificatonsModel", "");
    qmlRegisterUncreatableType<ConversationsModel>(uri, 1, 0, "ConversationsModel", "");
    qmlRegisterUncreatableType<ConversationModel>(uri, 1, 0, "ConversationModel", "");
}
