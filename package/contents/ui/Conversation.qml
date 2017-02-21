import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0
import QtQuick.Controls.Styles 1.4



Item
{
	Button
	{
		id: btnBack
		text: i18n("Back")
		onClicked:
		{
			stackView.pop()
		}
	}

	ListView
	{
		id: lstViewConversation
	    anchors.top: btnBack.bottom
	    anchors.bottom: txtFldNewMessage.top
	    anchors.right: parent.right
	    anchors.left: parent.left
	    spacing: 5
        
        Component.onCompleted:
	    {
 		   lstViewConversation.positionViewAtEnd()
		}

	    clip:true
	    model: bf.getBackend.conversationModel
	    delegate: 
	    Rectangle
        {
        	anchors.right: parent.right
	        anchors.left: parent.left
	        anchors.leftMargin: 10
	        anchors.rightMargin: 10
	        color: "#808080"
	        radius: 8
	       	height: childrenRect.height

        	Item
        	{
        		visible: model.isCompanion ? false : true
        		
        		anchors.margins: 15

        		anchors.right: parent.right
	        	anchors.left: parent.left
	        	height: childrenRect.height
	        	Image
	        	{
	        	    id: imgConvProfileImage_companion
	        	    source: model.senderImage
	        	}
	        	Text
	        	{
	        	    id: txtConversationUserName_companion
	        	    text: model.senderName
	        	    anchors.left: imgConvProfileImage_companion.left;
	        	    anchors.top: imgConvProfileImage_companion.bottom;
	        	    font.bold: true
	        	}
	        	Text 
	        	{
	        	    // use the defined model role "lastMessage"
	        	    text: model.text
	        	    wrapMode: Text.Wrap
	        	    anchors.right: parent.right
	        	    anchors.left: imgConvProfileImage_companion.right;
	        	    anchors.bottom: imgConvProfileImage_companion.bottom
	        	    anchors.leftMargin: 15	
	        	    elide: Text.ElideRight
	        	}

	        	//Component.onCompleted:
	    		//{
 		   		//	lstViewConversation.positionViewAtEnd()
				//}
	        }

	        Item
	        {
	        	visible: model.isCompanion ? true : false

	        	anchors.margins: 15

	        	anchors.right : parent.right
	        	anchors.left : parent.left
	        	height: childrenRect.height
	        	Image
	        	{
	        	    id: imgConvProfileImage_signed
	        	    source: model.senderImage
	        	    anchors.right: parent.right

	        	}
	        	Text
	        	{
	        	    id: txtConversationUserName_signed
	        	    text: model.senderName
	        	    anchors.right: imgConvProfileImage_signed.right;
	        	    anchors.top: imgConvProfileImage_signed.bottom;	        	    
	        	    font.bold: true
	        	}
	        	Text 
	        	{
	        	    // use the defined model role "lastMessage"
	        	    text: model.text
	        	    wrapMode: Text.Wrap
	        	    anchors.right: imgConvProfileImage_signed.left;
	        	    anchors.bottom: imgConvProfileImage_signed.bottom
	        	    anchors.rightMargin: 15	
	        	    elide: Text.ElideRight
	        	}
	        	//Component.onCompleted:
	    		//{
 		   		//	lstViewConversation.positionViewAtEnd()
				//}
	        }
	    }
	}

	TextField
    {
        id: txtFldNewMessage
        anchors.bottom: parent.bottom 
        anchors.left: parent.left
        anchors.right: btnSendMessage.left
        anchors.margins: 5
        placeholderText: qsTr(i18n("New message"))
        onEditingFinished: { 
                       			bf.getBackend.sendMessage(txtFldNewMessage.text)
                       			txtFldNewMessage.text = ""
                   			}
    }

    Button
    {
     id: btnSendMessage
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        text: i18n("Send")
        onClicked: { 
                       bf.getBackend.sendMessage(txtFldNewMessage.text)
                       txtFldNewMessage.text = ""
                   }
    }
}
