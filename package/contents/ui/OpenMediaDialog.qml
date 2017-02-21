import org.kde.private.weatherWidget 1.0

import QtQuick.Dialogs 1.1

FileDialog 
{
    id: fileDialog
    visible: true
    modality: Qt.WindowModal
    title: "Choose some files"
    selectExisting: true
    selectMultiple: true
    nameFilters: [ "Image files (*.png *.jpg)", "All files (*)" ]
    selectedNameFilter: "Image files (*.png *.jpg)"
    sidebarVisible: true
    onAccepted: {
        console.log("Accepted: " + fileUrls)
        for (var i = 0; i < fileUrls.length; ++i)
            console.log(fileUrls[i]);
    }
    onRejected: { console.log("Rejected") }
}  