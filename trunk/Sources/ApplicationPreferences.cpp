/****************************************************************
**
** Preferences
** 2008-12-01, Dr. Rainer Sieger
** last change: 2009-12-01
**
****************************************************************/

#include "Application.h"

const QString PrefDate = "PanTool, 2012-03-30";

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 11.3.2004

/*! @brief Sichern der Programmeinstellungen */

void MainWindow::savePreferences()
{
    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    settings.setValue( "WindowX", x() );
    settings.setValue( "WindowY", y() );
    settings.setValue( "WindowHeight", height() );
    settings.setValue( "WindowWidth", width() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    settings.setValue( "DialogX", posDialog.x() );
    settings.setValue( "DialogY", posDialog.y() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    settings.setValue( "NumOfProgramStarts", gi_NumOfProgramStarts );
    settings.setValue( "Path", gs_Path );
    settings.setValue( "FileFormat", gs_FilenameFormat );
    settings.setValue( "CodecInput", gi_CodecInput );
    settings.setValue( "CodecOutput", gi_CodecOutput );
    settings.setValue( "CodecDownload", gi_CodecDownload );
    settings.setValue( "EOL", gi_EOL );
    settings.setValue( "Extension", gi_Extension );
    settings.endGroup();

    // find area
    settings.beginGroup( "FindArea" );
    settings.setValue( "AreaDatabaseFilename", gs_AreaDatabaseFilename );
    settings.setValue( "DeleteInputFile", gb_fa_DeleteInputFile );
    settings.endGroup();

    // lat/long converter
    settings.beginGroup( "LatLongConverter" );
    settings.setValue( "ZoomLevel", gi_ll_ZoomLevel );
    settings.setValue( "MapType", gi_ll_MapType );
    settings.setValue( "LatitudeColumn", gi_ll_LatitudeColumn );
    settings.setValue( "LongitudeColumn", gi_ll_LongitudeColumn );
    settings.setValue( "NumberOfDigits", gi_ll_NumOfDigits );
    settings.setValue( "DecimalSeparator", gi_ll_DecimalSeparator );
    settings.setValue( "DeleteInputFile", gb_ll_DeleteInputFile );
    settings.endGroup();

    // Gauss-Krueger converter
    settings.beginGroup( "GaussKruegerConverter" );
    settings.setValue( "ZoomLevel", gi_gk_ZoomLevel );
    settings.setValue( "MapType", gi_gk_MapType );
    settings.setValue( "LatitudeColumn", gi_gk_LatitudeColumn );
    settings.setValue( "LongitudeColumn", gi_gk_LongitudeColumn );
    settings.setValue( "HochColumn", gi_gk_HochColumn );
    settings.setValue( "RechtsColumn", gi_gk_RechtsColumn );
    settings.setValue( "StreifenColumn", gi_gk_StreifenColumn );
    settings.setValue( "NumberOfDigitsLL", gi_gk_NumOfDigits_LL );
    settings.setValue( "NumberOfDigitsGK", gi_gk_NumOfDigits_GK );
    settings.setValue( "DecimalSeparator", gi_gk_DecimalSeparator );
    settings.setValue( "DeleteInputFile", gb_gk_DeleteInputFile );
    settings.endGroup();

    // UTM converter
    settings.beginGroup( "UTMConverter" );
    settings.setValue( "ZoomLevel", gi_utm_ZoomLevel );
    settings.setValue( "MapType", gi_utm_MapType );
    settings.setValue( "LatitudeColumn", gi_utm_LatitudeColumn );
    settings.setValue( "LongitudeColumn", gi_utm_LongitudeColumn );
    settings.setValue( "NorthingColumn", gi_utm_NorthingColumn );
    settings.setValue( "EastingColumn", gi_utm_EastingColumn );
    settings.setValue( "ZoneColumn", gi_utm_ZoneColumn );
    settings.setValue( "ReferenceEllipsoid", gi_utm_ReferenceEllipsoid );
    settings.setValue( "NumberOfDigitsLL", gi_utm_NumOfDigits_LL );
    settings.setValue( "NumberOfDigitsUTM", gi_utm_NumOfDigits_UTM );
    settings.setValue( "DecimalSeparator", gi_utm_DecimalSeparator );
    settings.setValue( "DeleteInputFile", gb_utm_DeleteInputFile );
    settings.endGroup();

    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 21.11.2007

/*! @brief Laden der Programmeinstellungen */

void MainWindow::loadPreferences()
{
    #if defined(Q_OS_LINUX)
        gi_CodecInput                       = _UTF8_;               //!< UTF-8
        gi_CodecOutput                      = _UTF8_;
        gi_CodecDownload                    = _UTF8_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_MAC)
        gi_CodecInput                       = _APPLEROMAN_;        //!< Apple Roman
        gi_CodecOutput                      = _APPLEROMAN_;
        gi_CodecDownload                    = _APPLEROMAN_;
        gi_EOL                              = _UNIX_;
    #endif

    #if defined(Q_OS_WIN)
        gi_CodecInput                       = _LATIN1_;            //!< Latin-1 = ISO 8859-1
        gi_CodecOutput                      = _LATIN1_;
        gi_CodecDownload                    = _LATIN1_;
        gi_EOL                              = _WIN_;
    #endif

    #if defined(Q_OS_LINUX)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    #if defined(Q_OS_MAC)
        QSettings settings( getPreferenceFilename(), QSettings::NativeFormat );
    #endif

    #if defined(Q_OS_WIN)
        QSettings settings( getPreferenceFilename(), QSettings::IniFormat );
    #endif

    settings.beginGroup( QCoreApplication::applicationName() );

    settings.beginGroup( "Window" );
    posWindow.setX( settings.value( "WindowX", 100 ).toInt() );
    posWindow.setY( settings.value( "WindowY", 100 ).toInt() );
    sizeWindow.setHeight( settings.value( "WindowHeight", 250 ).toInt() );
    sizeWindow.setWidth( settings.value( "WindowWidth", 600 ).toInt() );
    settings.endGroup();

    settings.beginGroup( "Dialog" );
    posDialog.setX( settings.value( "DialogX", 110 ).toInt() );
    posDialog.setY( settings.value( "DialogY", 110 ).toInt() );
    settings.endGroup();

    // General settings
    settings.beginGroup( "GeneralSettings" );
    gi_NumOfProgramStarts = settings.value( "NumOfProgramStarts", 0 ).toInt();
    gs_Path               = settings.value( "Path", "" ).toString();
    gs_FilenameFormat     = settings.value( "FileFormat", "zz%a_%N" ).toString();
    gi_CodecInput         = settings.value( "CodecInput", gi_CodecInput ).toInt();
    gi_CodecOutput        = settings.value( "CodecOutput", gi_CodecOutput ).toInt();
    gi_CodecDownload      = settings.value( "CodecDownload", gi_CodecDownload ).toInt();
    gi_EOL                = settings.value( "EOL", gi_EOL ).toInt();
    gi_Extension          = settings.value( "Extension", _TXT_ ).toInt();
    settings.endGroup();

    // find area
    settings.beginGroup( "FindArea" );
    gs_AreaDatabaseFilename = settings.value( "AreaDatabaseFilename", "Browse through the area database with \"Browse ...\"" ).toString();
    gb_fa_DeleteInputFile   = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // lat/long converter
    settings.beginGroup( "LatLongConverter" );
    gi_ll_ZoomLevel        = settings.value( "ZoomLevel", 5 ).toInt();
    gi_ll_MapType          = settings.value( "MapType", 1 ).toInt();
    gi_ll_LatitudeColumn   = settings.value( "LatitudeColumn", 1 ).toInt();
    gi_ll_LongitudeColumn  = settings.value( "LongitudeColumn", 2 ).toInt();
    gi_ll_NumOfDigits      = settings.value( "NumberOfDigits", 5 ).toInt();
    gi_ll_DecimalSeparator = settings.value( "DecimalSeparator", _DOT_ ).toInt();
    gb_ll_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // Gauss-Krueger converter
    settings.beginGroup( "GaussKruegerConverter" );
    gi_gk_ZoomLevel        = settings.value( "ZoomLevel", 5 ).toInt();
    gi_gk_MapType          = settings.value( "MapType", 1 ).toInt();
    gi_gk_LatitudeColumn   = settings.value( "LatitudeColumn", 1 ).toInt();
    gi_gk_LongitudeColumn  = settings.value( "LongitudeColumn", 2 ).toInt();
    gi_gk_HochColumn       = settings.value( "HochColumn", 1 ).toInt();
    gi_gk_RechtsColumn     = settings.value( "RechtsColumn", 2 ).toInt();
    gi_gk_StreifenColumn   = settings.value( "StreifenColumn", 3 ).toInt();
    gi_gk_NumOfDigits_LL   = settings.value( "NumberOfDigitsLL", 5 ).toInt();
    gi_gk_NumOfDigits_GK   = settings.value( "NumberOfDigitsGK", 1 ).toInt();
    gi_gk_DecimalSeparator = settings.value( "DecimalSeparator", _DOT_ ).toInt();
    gb_gk_DeleteInputFile  = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();

    // UTM converter
    settings.beginGroup( "UTMConverter" );
    gi_utm_ZoomLevel          = settings.value( "ZoomLevel", 5 ).toInt();
    gi_utm_MapType            = settings.value( "MapType", 1 ).toInt();
    gi_utm_LatitudeColumn     = settings.value( "LatitudeColumn", 1 ).toInt();
    gi_utm_LongitudeColumn    = settings.value( "LongitudeColumn", 2 ).toInt();
    gi_utm_NorthingColumn     = settings.value( "NorthingColumn", 1 ).toInt();
    gi_utm_EastingColumn      = settings.value( "EastingColumn", 2 ).toInt();
    gi_utm_ZoneColumn         = settings.value( "ZoneColumn", 3 ).toInt();
    gi_utm_ReferenceEllipsoid = settings.value( "ReferenceEllipsoid", 22 ).toInt();
    gi_utm_NumOfDigits_LL     = settings.value( "NumberOfDigitsLL", 5 ).toInt();
    gi_utm_NumOfDigits_UTM    = settings.value( "NumberOfDigitsUTM", 1 ).toInt();
    gi_utm_DecimalSeparator   = settings.value( "DecimalSeparator", _DOT_ ).toInt();
    gb_utm_DeleteInputFile    = settings.value( "DeleteInputFile", false ).toBool();
    settings.endGroup();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Ermitteln des Dateinamens fuer die Programmeinstellungen.
*   @return Name der Datei fuer die Programmeinstellungen. Dateiname ist gleich "_ERROR_",
*   wenn Betriebssystem ungleich Windows, Mac OS oder Unix ist.
*/

QString MainWindow::getPreferenceFilename()
{
    QString s_PrefFilename = "";

    #if defined(Q_OS_LINUX)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/.config/" ) + QCoreApplication::organizationName() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".conf" );
    #endif

    #if defined(Q_OS_MAC)
        s_PrefFilename = QDir::homePath() + QLatin1String( "/Library/Preferences/" ) + QCoreApplication::organizationDomain().section( ".", 1, 1 ) + "." + QCoreApplication::organizationDomain().section( ".", 0, 0 ) + "." + QCoreApplication::applicationName() + QLatin1String( ".plist" );
    #endif

    #if defined(Q_OS_WIN)
        if ( QCoreApplication::applicationName().toLower().endsWith( "portable" ) )
        {
            s_PrefFilename = QCoreApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
        else
        {
            QSettings cfg( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName() );
            s_PrefFilename = QFileInfo( cfg.fileName() ).absolutePath() + "/" + QCoreApplication::applicationName() + "/" + QCoreApplication::applicationName() + QLatin1String( ".ini" );
        }
    #endif

    return( s_PrefFilename );
}
