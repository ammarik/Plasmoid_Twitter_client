import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0

Item 
{
    id: containerTabNotifications
    anchors.fill: parent

    ListView
    {
        id: listViewNotifiactions
        anchors.fill: parent
        clip:true
        model: bf.getBackend.notificationsModel
        delegate: 
        Rectangle
        {
            color: index % 2 == 0 ? "#808080" : "#909090"

            Text
            {
                id: txtTabNotifiUsers
                text: model.name
                font.bold: true
                wrapMode: Text.Wrap
                anchors.leftMargin: 15
            }

            Text 
            {
                // use the defined model role "text"
                text: model.text
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                anchors.left: txtTabNotifiUsers.right
                anchors.right: parent.right
            }
            anchors.right : parent.right
            anchors.left : parent.left
            height: childrenRect.height
      }

        anchors.right : parent.right
        anchors.left : parent.left
    }

}



//Item {
//    id: container
//    anchors.fill: parent
//    
//
//    property var cacheBackend: null
//
//    Text
//    {
//        id: txt
//        text: "Hello"
//        x: 220
//        y: 90
//    }
//
//    MouseArea 
//    {
//        id: mousearea1
//        anchors.fill: parent
//    }
//
//    Component.onCompleted:
//    {
//        txt.text = Qt.binding(function() 
//        { 
//            return mousearea1.pressed ? backend.isLoggedIn : "Klikni!!"
//        });
//    }
//    
//    Rectangle 
//    {
//        id: colorbutton
//        width: 200; height: 80;
//
//        MouseArea 
//        {
//            id: mousearea
//            anchors.fill: parent
//        }
//
//        Component.onCompleted:
//        {
//            color = Qt.binding(function() { return mousearea.pressed ? "steelblue" : "lightsteelblue"});
//        }
//    }
//}