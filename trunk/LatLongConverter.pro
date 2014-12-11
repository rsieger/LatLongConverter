# LatLongConverter - Latitude/Longitude converter
# Rainer Sieger
# Alfred Wegener Institute, Bremerhaven, Germany
# last change: 2014-10-04

macx {
    message( "MacOS X" )

    # creating cache file
    cache()

    # Set SDK
    QMAKE_MAC_SDK = macosx10.9

    # Only Intel binaries are accepted so force this
    CONFIG += x86_64

    # Minimum OS X version for submission is 10.6.6
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

    # Icon
    ICON = ./Resources/icon/Application.icns

    # Replace default Info.plist
    QMAKE_INFO_PLIST = ./Resources/Info.plist

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_OBJECTIVE_CFLAGS_RELEASE =  $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32 {
    message( "Windows" )

    # Icon
    RC_FILE = ./Resources/Application.rc
}

linux-g++ {
    message( "Linux" )
}

TARGET = LatLongConverter
TEMPLATE = app

QT += widgets network

INCLUDEPATH += ./Headers

HEADERS = ./Headers/Application.h \
    ./Headers/Globals.h \
    ./Headers/Webfile.h \
    ./Headers/Ellipsoid.h \
    ./Headers/findArea.h \
    ./Headers/convertPosition.h \
    ./Forms/AboutDialog/AboutDialog.h \
    ./Forms/FindAreaDialog/FindAreaDialog.h \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.h \
    ./Forms/GaussKruegerDialog/GaussKruegerDialog.h \
    ./Forms/GaussKruegerOptionsDialog/GaussKruegerOptionsDialog.h \
    ./Forms/LatLongDialog/LatLongDialog.h \
    ./Forms/LatLongOptionsDialog/LatLongOptionsDialog.h \
    ./Forms/UTMDialog/UTMDialog.h \
    ./Forms/UTMOptionsDialog/UTMOptionsDialog.h

SOURCES = ./Sources/ApplicationInit.cpp \
    ./Sources/ApplicationMainWindow.cpp \
    ./Sources/ApplicationErrors.cpp \
    ./Sources/ApplicationPreferences.cpp \
    ./Sources/ApplicationCreateMenu.cpp \
    ./Sources/convertPosition.cpp \
    ./Sources/findArea.cpp \
    ./Sources/findPolygonCentroid.cpp \
    ./Sources/LatLongConverter.cpp \
    ./Sources/GaussKruegerConverter.cpp \
    ./Sources/UTMConverter.cpp \
    ./Sources/Webfile.cpp \
    ./Forms/AboutDialog/AboutDialog.cpp \
    ./Forms/FindAreaDialog/FindAreaDialog.cpp \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.cpp \
    ./Forms/GaussKruegerDialog/GaussKruegerDialog.cpp \
    ./Forms/GaussKruegerOptionsDialog/GaussKruegerOptionsDialog.cpp \
    ./Forms/LatLongDialog/LatLongDialog.cpp \
    ./Forms/LatLongOptionsDialog/LatLongOptionsDialog.cpp \
    ./Forms/UTMDialog/UTMDialog.cpp \
    ./Forms/UTMOptionsDialog/UTMOptionsDialog.cpp

FORMS = ./Forms/AboutDialog/aboutdialog.ui \
    ./Forms/FindAreaDialog/findareadialog.ui \
    ./Forms/GeneralOptionsDialog/generaloptionsdialog.ui \
    ./Forms/GaussKruegerDialog/gausskruegerdialog.ui \
    ./Forms/GaussKruegerOptionsDialog/gausskruegeroptionsdialog.ui \
    ./Forms/LatLongDialog/latlongdialog.ui \
    ./Forms/LatLongOptionsDialog/latlongoptionsdialog.ui \
    ./Forms/UTMDialog/utmdialog.ui \
    ./Forms/UTMOptionsDialog/utmoptionsdialog.ui
