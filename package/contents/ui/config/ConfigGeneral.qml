import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0
import QtQuick.Controls.Styles 1.4

Item {
    
    BackendFactory
    {
        id: bf
    }

    Item {        
        Item
        {
            id: loginProperty
            visible: bf.getBackend.isLoggedIn ? false : true

            Label
            {
                id: lblRegApp
                text: i18n("Register App:")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                anchors.top: parent.top
                anchors.margins: 5
            }

            TextField
            {
                id: txtFldConsumerKey
                placeholderText: qsTr(i18n("Consumer Key (API Key)"))
                anchors.left: lblRegApp.right
                anchors.top: lblRegApp.top
                width: 200
                anchors.margins: 5
            }

            TextField
            {
                id: txtFldConsumerSecret
                placeholderText: qsTr(i18n("Consumer Secret (API Secret)"))
                anchors.left: lblRegApp.right
                anchors.top: txtFldConsumerKey.bottom
                width: 200
                anchors.margins: 5
            }

            Button
            {
                id:btnSetConsumerCred
                text: i18n("Set API credentials")
                anchors.left: lblRegApp.right
                anchors.top: txtFldConsumerSecret.bottom
                anchors.margins: 5

                onClicked: 
                {
                    bf.getBackend.setConsumerCred(txtFldConsumerKey.text, txtFldConsumerSecret.text);
                    txtFldConsumerSecret.text = ""
                    txtFldConsumerKey.text = ""
                }
            }

            Text 
            { 
                id: textCredInfo
                color: "red"
                text: bf.getBackend.credInfo //"Optional, but recommended to change API credentials!"
                anchors.top: btnSetConsumerCred.bottom
                anchors.margins: 5
            }

            Label
            {
                id: lblLogin
                text: i18n("Login:")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                anchors.top: textCredInfo.bottom
                anchors.margins: 5
                anchors.topMargin: 15
            }

            Button
            {
                id:btnGeneratePin
                text: i18n("Generate PIN")
                onClicked: textAuthLink.text = bf.getBackend.AuthLink
                anchors.left: lblRegApp.right
                anchors.top: lblLogin.top
                anchors.margins: 5
            }

            Text 
            { 
                id: textAuthLink
                onLinkActivated: Qt.openUrlExternally(link)
                anchors.left: btnGeneratePin.right
                anchors.bottom: btnGeneratePin.bottom
                anchors.margins: 5
    
                MouseArea 
                {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

            TextField
            {
                id: txtFldPin
                placeholderText: qsTr(i18n("PIN"))
                anchors.left: lblRegApp.right
                anchors.top: btnGeneratePin.bottom
                anchors.margins: 5
                validator: RegExpValidator { regExp: /^[0-9]{7}$/ }
            }
        
            Button
            {
                id: btnLogin
                text: i18n("Login")
                anchors.left: lblRegApp.right
                anchors.top: txtFldPin.bottom
                anchors.margins: 5

                onClicked: {  
                    if(txtFldPin.acceptableInput)
                    {
                        bf.getBackend.login(txtFldPin.text)
                        txtFldPin.text = ""
                        bf.getBackend.authenticate()
                        bf.getBackend.startStreamConsum()
                        logoutItem.visible = bf.getBackend.isLoggedIn ? true : false 
                        loginProperty.visible = bf.getBackend.isLoggedIn ? false : true
                    }
                }
            }
        }

        Item
        {
            id: logoutItem
            visible: bf.getBackend.isLoggedIn ? true : false

            Button
            {
                id: btnLogout
                text: i18n("Logout")
                onClicked:  {
                                bf.getBackend.logout()
                                loginProperty.visible = bf.getBackend.isLoggedIn ? false : true
                                logoutItem.visible = bf.getBackend.isLoggedIn ? true : false
                            }
            }
        }
    }
} 

