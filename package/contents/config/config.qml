import QtQuick 2.2
import org.kde.plasma.configuration 2.0
import "../ui" as MyUi

ConfigModel 
{
    ConfigCategory 
    {
        name: i18n('General')
        icon: 'preferences-other'
        source: 'config/ConfigGeneral.qml'
    }
    ConfigCategory 
    {
    	name: i18n('Notifications')
    	icon: 'preferences-desktop-notification'
    	source: 'config/ConfigNotifications.qml'
    }
} 
