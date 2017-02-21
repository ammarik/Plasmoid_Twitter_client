import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0
import QtQuick.Controls.Styles 1.4

///Represets main screen of plasmoid 
Item 
{
    id: main

    BackendFactory
    {
        id: bf
    }

    TabView
    {
        id: tabView
        anchors.top: parent.top
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom
        tabsVisible: true

        Tab 
        {
            title: i18n("Tweets")
            onVisibleChanged:  bf.getBackend.selectedTab(tabView.currentIndex);
            TabTweets { }
        }
        Tab 
        {
            id: tabNotif
            title: i18n("Notifications")
            onVisibleChanged:  bf.getBackend.selectedTab(tabView.currentIndex);
            TabNotifications { }
        }
        Tab
        {
            id: tabMessages
            title: i18n("Messages")
            onVisibleChanged:  bf.getBackend.selectedTab(tabView.currentIndex);
            TabMessages { }
        }
    }

    Connections
    {
        target: bf.getBackend
        onLatestNotifsChanged: tabNotif.title = i18n("Notifications ") + latestNotifsIndicator
        onLatestMessagChanged: tabMessages.title = i18n("Messages ") + latestMessagIndicator
    }    
}
