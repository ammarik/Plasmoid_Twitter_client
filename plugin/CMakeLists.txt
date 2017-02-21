set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    qmlBackendFactory.cpp
    backend.cpp
    config.cpp

    ./Twitter/Twitter.cpp
    ./Twitter/ImageDownloader.cpp
    ./Twitter/TwitterWorker.cpp
    ./Twitter/twitterTimer.cpp
    ./Twitter/twitterRequester.cpp

    ./Model/listItem.h
    ./Model/subListedListItem.h

    ./Model/model.cpp
    ./Model/usersContainer.cpp
    ./Model/user.cpp
    ./Model/listModel.cpp
    ./Model/subListedListModel.cpp

    ./Model/statusesModel.cpp
    ./Model/status.cpp

    ./Model/notificationsModel.cpp
    ./Model/notification.cpp

    ./Model/conversationsModel.cpp
    ./Model/conversation.cpp

    ./Model/conversationModel.cpp
    ./Model/message.cpp

    ./Model/tag.cpp
    ./Model/hashtag.cpp
    ./Model/mention.cpp
    ./Model/url.cpp
    ./Model/media.cpp

    ./Model/ModelExceptions/missingSignedUserException.cpp
    
    ./Exceptions/createFolderException.cpp
    ./Exceptions/myFileException.cpp
    ./Exceptions/permFileException.cpp

    ./JsonCpp/jsoncpp.cpp
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})

include_directories(./Twitter/include) # -I flags for compiler
link_directories(/usr/local/lib/)        # -L flags for linker

target_link_libraries(plasmoidplugin Qt5::Quick Qt5::Widgets KF5::Plasma KF5::I18n twitcurl curl oauth )

install(TARGETS plasmoidplugin DESTINATION ${QML_INSTALL_DIR}/cz/cvut/fit/twitterPlasmoid)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/cz/cvut/fit/twitterPlasmoid)
