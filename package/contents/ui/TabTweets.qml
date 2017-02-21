import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Controls 1.4
import cz.cvut.fit.twitterPlasmoid 1.0



Item 
{
    id: container
    anchors.fill: parent

    ListView
    {
        id: listviewTweets
        anchors
        {
            top : parent.top
            bottom: txtFldNewTweeet.top
            right : parent.right
            left : parent.left
        }
        clip:true
        model: bf.getBackend.statusesModel
        delegate: 
        Rectangle
        {
            color: index % 2 == 0 ? "#808080" : "#909090"
            Image
            {
                id: imgTabTweetsProfileImage
                source: model.image//"../images/default_profile.png"
            }

            Text
            {
                id: txtTabTweetsUserName
                text: model.name
                anchors.left: imgTabTweetsProfileImage.right;
                font.bold: true
                anchors.leftMargin: 15

                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
                MouseArea 
                {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

            Text
            {
                id: txtTabTweetsRtUserName
                text: model.is_retweet ? "Retweeted " + model.retweeteds_user_name : ""
                visible: model.is_retweet ? true : false
                anchors.left: imgTabTweetsProfileImage.right
                anchors.top: txtTabTweetsUserName.bottom
                anchors.leftMargin: 15

                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
                MouseArea 
                {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

            Text 
            {
                // use the defined model role "text"
                text: model.text
                wrapMode: Text.Wrap
                anchors.left: imgTabTweetsProfileImage.right;
                anchors.top: model.is_retweet ? txtTabTweetsRtUserName.bottom : txtTabTweetsUserName.bottom
                anchors.topMargin: 2
                anchors.leftMargin: 15

                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
                MouseArea 
                {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }

                anchors.right: parent.right
                elide: Text.ElideRight
            }
            anchors.right : parent.right
            anchors.left : parent.left
            height: childrenRect.height
      }
    }


    TextField
    {
        id: txtFldNewTweeet
        anchors.bottom: parent.bottom 
        anchors.left: parent.left
        anchors.right: btnNewTweet.left
        anchors.margins: 5
        maximumLength: 140
        placeholderText: qsTr(i18n("Post new tweet!"))
        onEditingFinished: { 
                       bf.getBackend.postNewStatus(txtFldNewTweeet.text)
                       txtFldNewTweeet.text = ""
                   }
    }

    Button
    {
     id: btnNewTweet
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        text: "Tweet!"
        onClicked: { 
                       bf.getBackend.postNewStatus(txtFldNewTweeet.text)
                       txtFldNewTweeet.text = ""
                   }
    }

    Component.onCompleted: 
    {
        bf.getBackend.startStreamConsum()
    }  
}



