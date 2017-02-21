import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0
import QtQuick.Controls.Styles 1.4

Item 
{ 
    BackendFactory
    {
        id: bf
    }

    Item
    {
        CheckBox 
        {
            id: chckb_enableNotif
            text: qsTr(i18n("Enable notifications"))
            checked: true
            onClicked:  bf.getBackend.notificationsEnabled(chckb_enableNotif.checked)
        }   
    }

    Component.onCompleted: 
    {
        //chckb_enableNotif.checked: true;
         bf.getBackend.notificationsEnabled(chckb_enableNotif.checked);
    }
} 

