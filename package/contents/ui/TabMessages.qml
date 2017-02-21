import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0

StackView
{
	id: stackView
	initialItem: containerConversations

	Item 
	{
	    id: containerConversations
	    ListView
	    {
	        id: view
	        anchors.fill: parent
	        clip:true
	        model: bf.getBackend.conversationsModel
	        delegate: 
	        Rectangle
	        {
	            color: index % 2 == 0 ? "#808080" : "#909090"
	            Image
	            {
	                id: imgTabMessagesProfileImage
	                source: model.image
	            }
	
	            Text
	            {
	                id: txtTabMessagesUserName
	                text: model.name
	                anchors.left: imgTabMessagesProfileImage.right;
	                font.bold: true
	                anchors.leftMargin: 15
	            }
	
	            Text 
	            {
	                // use the defined model role "lastMessage"
	                text: model.lastMessage
	                wrapMode: Text.Wrap
	                anchors.left: imgTabMessagesProfileImage.right;
	                anchors.top: txtTabMessagesUserName.bottom
	                anchors.leftMargin: 25
	
	                anchors.right: parent.right
	                elide: Text.ElideRight
	            }
	            anchors.right : parent.right
	            anchors.left : parent.left
	            height: childrenRect.height

	
	            MouseArea 
	            {
	            	anchors.fill: parent
	            	onClicked :
	            	{	
	            		bf.getBackend.setConversationId = model.conversationId
	            		stackView.push({item:Qt.resolvedUrl("Conversation.qml"), destroyOnPop:false})
	           		}
	        	}
	      	}
	    }
	}
}