cmake_minimum_required(VERSION 3.16)

project(SCClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)
include_directories(data)
include_directories(net)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appSCClient
    main.cpp
)

qt_add_qml_module(appSCClient
    URI SCClient
    QML_FILES Main.qml
    QML_FILES Map.qml

    RESOURCES images/RoadMap.jpg
    SOURCES data/datacontext.h data/datacontext.cpp
    SOURCES net/netcontext.h net/netcontext.cpp
    QML_FILES CrossingSet.qml
    QML_FILES CarSet.qml
    QML_FILES CarGroup.qml
    QML_FILES CrossingGroup.qml
    SOURCES systemcontrol.h systemcontrol.cpp
    RESOURCES images/car.png
    QML_FILES server/ServerSet.qml
    QML_FILES
    QML_FILES
    QML_FILES
    QML_FILES
    QML_FILES
    QML_FILES

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSCClient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSCClient
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSCClient
    PRIVATE
        Qt6::Quick
        Qt6::Network
)

install(TARGETS appSCClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
