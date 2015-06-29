/* Application.h              */
/* 2013-05-15                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QTextEdit>
#include <QTextCodec>
#include <QTextStream>
#include <QStringListModel>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QTimer>
#include <QMimeData>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QStatusBar>
#include <QSettings>
#include <qmath.h>

#include "Globals.h"
#include "Webfile.h"
#include "findArea.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QLabel;
class QProgressDialog;
class QProgressBar;
class QTimer;
class QDateTime;
class QUrl;
class QTextEdit;
class QStringListModel;
class QListView;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "http://www.pangaea.de" )

    struct structLocation
    {
        double	Latitude;
        double	Longitude;
    };

    struct structArea
    {
        QString	AreaName;       // Name of area
        int     NumOfPoints;    // Number of points in closed polygon
        float   minLat;         // Minimum latitude
        float   maxLat;         // Maximum latitude
        float   minLong;        // Minimum longitude
        float   maxLong;        // Maximum longitude
    };

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    QStringList gsl_FilenameList;                   //!< Liste aller Dateinamen incl. Pfad.

    // General settings
    QString     gs_Path;                            //!< Der zuletzt verwendete Pfad.
    QString     gs_FilenameFormat;                  //!< Bestimmt in die Form des automatisch erzeugten neuen Dateinamens. \%a = aktuelle Actionnumber, \%N = Dateiname.
    QString     gs_Version;                         //!< Aktuelle Version des Programms.

    int         gi_ActionNumber;                    //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.
    int         gi_NumOfProgramStarts;              //!< Anzahl der Programmstarts
    int         gi_CodecInput;                      //!< Encoding der Inputdateien
    int         gi_CodecOutput;                     //!< Encoding der Outputdateien
    int         gi_CodecDownload;                   //!< Encoding der Downloaddateien
    int         gi_EOL;                             //!< End-of-Line Zeichen fr die Outputdateien
    int         gi_Extension;                       //! Dateierweiterung fr Ausgabedateien

    // find area
    QString     gs_AreaDatabaseFilename;             //!< Name der Area database
    bool        gb_fa_DeleteInputFile;               //!< loesche Inputdatei

    // lat/long single converter
    int         gi_ll_ZoomLevel;                     //!< Zoom level fuer Nokia Map
    int         gi_ll_MapType;                       //!< Map type

    // lat/long batch converter
    int         gi_ll_LatitudeColumn;                //!< Spalte fuer Latitude
    int         gi_ll_LongitudeColumn;               //!< Spalte fuer Longitude
    int         gi_ll_NumOfDigits;                   //!< Ausgabegenauigkeit
    int         gi_ll_DecimalSeparator;              //!< Dezimalzeichen
    bool        gb_ll_DeleteInputFile;               //!< loesche Inputdatei.

    // Gauss-Krueger single converter
    int         gi_gk_ZoomLevel;                     //!< Zoom level fuer Nokia Map
    int         gi_gk_MapType;                       //!< Map type

    // Gauss-Krueger batch converter
    int         gi_gk_LatitudeColumn;                //!< Spalte fuer Latitude
    int         gi_gk_LongitudeColumn;               //!< Spalte fuer Longitude
    int         gi_gk_HochColumn;                    //!< Spalte fuer Hoch-Wert
    int         gi_gk_RechtsColumn;                  //!< Spalte fuer Rechts-Wert
    int         gi_gk_StreifenColumn;                //!< Spalte fuer GK-Streifen
    int         gi_gk_NumOfDigits_LL;                //!< Ausgabegenauigkeit
    int         gi_gk_NumOfDigits_GK;                //!< Ausgabegenauigkeit
    int         gi_gk_DecimalSeparator;              //!< Dezimalzeichen
    bool        gb_gk_DeleteInputFile;               //!< loesche Inputdatei.

    // UTM single converter
    int         gi_utm_ZoomLevel;                    //!< Zoom level fuer Nokia Map
    int         gi_utm_MapType;                      //!< Map type

    // UTM batch converter
    int         gi_utm_LatitudeColumn;               //!< Spalte fuer Latitude
    int         gi_utm_LongitudeColumn;              //!< Spalte fuer Longitude
    int         gi_utm_NorthingColumn;               //!< Spalte fuer Northing-Wert
    int         gi_utm_EastingColumn;                //!< Spalte fuer Easting-Wert
    int         gi_utm_ZoneColumn;                   //!< Spalte fuer UTM-Zone
    int         gi_utm_ReferenceEllipsoid;           //!< ID des Reference Ellipsoids
    int         gi_utm_NumOfDigits_LL;               //!< Ausgabegenauigkeit
    int         gi_utm_NumOfDigits_UTM;              //!< Ausgabegenauigkeit
    int         gi_utm_DecimalSeparator;             //!< Dezimalzeichen
    bool        gb_utm_DeleteInputFile;              //!< loesche Inputdatei.


    int findArea( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, QVector<AreaItem> &Area, QVector<PositionItem> &Position, const bool DeleteInputFile = false, const int NumOfFiles = 0 );
    int readAreaDataBaseFile(const QString &FilenameADB, QVector<AreaItem> &Area, QVector<PositionItem> &Position );
    int buildAreaDatabase( const QString& FilenameIn, const int CodecInput, const int NumOfFiles );
    int PtInPolygon( double d_Latitude, double d_Longitude, int StartPosition, int NumOfPoints, QVector<PositionItem> &v_Position );
    int polyCentroid( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const bool DeleteInputFile, const int NumOfFiles );

    int LatLongConverter( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int ColumnLatitude, const int ColumnLongitude, const int NumOfDigits, const int DecimalSeparator, const bool DeleteInputFile, const int NumOfFiles );
    int GaussKruegerConverter( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int ColumnLatitude, const int ColumnLongitude, const int ColumnHoch, const int ColumnRechts, const int ColumnStreifen, const int NumOfDigits, const int DecimalSeparator, const bool DeleteInputFile, const int NumOfFiles );
    int UTMConverter( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int ColumnLatitude, const int ColumnLongitude, const int ColumnNorthing, const int ColumnEasting, const int ColumnZone, const int ReferenceEllipsoid, const int NumOfDigits, const int DecimalSeparator, const bool DeleteInputFile, const int NumOfFiles );

    int doFindAreaDialog( QString &AreaDatabaseFilename, bool &DeleteInputFile );
    int doGaussKruegerOptionsDialog( const int Mode, int &ColumnLat, int &ColumnLong, int &ColumnHoch, int &ColumnRechts, int &ColumnStreifen, int &NumOfDigits, int &DecimalSeparator, bool &DeleteInputFile );
    int doLatLongOptionsDialog( int &ColumnLat, int &ColumnLong, int &NumOfDigits, int &DecimalSeparator, bool &DeleteInputFile );
    int doUTMOptionsDialog( const int Mode, int &ColumnLat, int &ColumnLong, int &ColumnNorthing, int &ColumnEasting, int &ColumnZone, int &ReferenceEllipsoid, int &NumOfDigits, int &DecimalSeparator, bool &DeleteInputFile );

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
    void closeEvent( QCloseEvent *event );
    void onError( const int err );

signals:
    void finishedBuildFilelist( bool );

private slots:
    int  chooseFile();
    int  chooseFiles();
    int  chooseFolder( const QString &Folder = "" );
    int  saveFile();
    int  saveFileAs();
    void hideWindow();
    void newWindow();
    void displayHelp();
    void about();
    void clearStatusMessage();

    void doLatLongConverterFile();
    void doLatLongConverterDialog();
    void doLLtoGKConverterFile();
    void doGKtoLLConverterFile();
    void doGKConverterDialog();
    void doUTMtoLLConverterFile();
    void doLLtoUTMConverterFile();
    void doUTMConverterDialog();
    void doFindArea();
    void doFindPolygonCentroid();
    void doBuildAreaDatabase();

// Dialogs
    void doLatLongDialog();
    void doGaussKruegerDialog();
    void doUTMDialog();
    int  doGeneralOptionsDialog();

private:
    QStringList expandCommandline();
    QStringList renameFiles( const QStringList Filename, const QString &searchStr, const QString &replaceStr );
    bool buildFilename( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QString &Filename, QString &FilenameIn, QString &FilenameOut );
    bool existsFirstFile( const int ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList );
    bool isEmptyLine( const QString &String );
    int NumOfSections( const QString &String );
    int buildFileList( QStringList &FilenameList, const QString &Filename, const bool extractZipFile = true );
    int calcFileSizeClass( const QString &FilenameIn, const int NumOfFiles, const int lowerLimit = 100, const int upperLimit = 1000 );
    int copyStringList( const QStringList InStringList, QStringList &OutStringList, const QString &s_Pattern = "*" );
    int emptyDir( const QString &Dir );
    int extractZipFile( const QString &Filename, const bool createNewDir = false, const bool delZipFile = false );
    int incFileProgress( const int NumOfFiles, const int FileNumber );
    int incProgress( const int NumOfFiles, const int Step );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const int NumOfFiles = 0, const qint64 Bytes = 0 );
    int removeFile( const QString &Filename );
    int downloadFile( const QString &Url, const QString &absoluteFilePath );
    unsigned int incProgress( const int NumOfFiles, const unsigned int filesize, const unsigned int length, const QString &InputStr );
    void appendItem( QStringList &List, const QString &Item, const QString &SS = "", const QString &RS = "" );
    void clearFilenameList( int &ActionNumber, QStringList &FilenameList );
    void clearList( QStringList& List );
    void clearMessage();
    void createActions();
    void createMenus();
    void createStatusBar();
    void endTool( const int err, const int stopProgress, int &ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList, const QString &doneMessage = "Done", const QString &canceledMessage = "Converter was canceled", const bool clearList = false, const bool incActionNumber = true );
    void initFileProgress( const int NumOfFiles, const QString &FilenameIn, const QString &MessageText );
    void initProgress( const int NumOfFiles, const QString &Filename, const QString &MessageText, const int totalNumberOfSteps );
    void initProgress( const int totalNumberOfSteps );
    void listDir( QStringList &Filename, const QString &Directory, const bool extractZipFile = true );
    void loadPreferences();
    void resetFileProgress( const int NumOfFiles );
    void resetProgress( const int NumOfFiles = 0 );
    void savePreferences();
    void setNormalCursor();
    void setStatusBar( const QString &Message = "", const int seconds = 0 );
    void setStatusBarFileInProgress( const QString &Filename, const QString &Message = "", const int i_seconds = 0 );
    void setWTitle( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void setWaitCursor();
    void showFilenameList( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList FilenameList );
    void showList( const QStringList List );
    void showMessage( const QString &Message, QStringList &MessageList );
    void wait( const int msec );

    QString getApplicationName( const bool replaceUnderline = false );
    QString getDataLocation();
    QString getDocumentDir();
    QString getPreferenceFilename();
    QString getVersion();
    QString setEOLChar( const int EOL );
    QString setExtension( const int Extension );

    QMenu   *fileMenu;
    QMenu   *basicToolsMenu;
    QMenu   *specialToolsMenu;
    QMenu   *helpMenu;

    QAction *newWindowAction;
    QAction *openFileAction;
    QAction *openFolderAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *hideWindowAction;
    QAction *setOptionsAction;
    QAction *exitAction;
    QAction *getFilesAction;

    QAction *findAreaAction;
    QAction *findPolygonCentroidAction;
    QAction *buildAreaDatabaseAction;
    QAction *GaussKruegerConverterGKtoLLFileAction;
    QAction *GaussKruegerConverterLLtoGKFileAction;
    QAction *GaussKruegerConverterDialogAction;
    QAction *UTMConverterUTMtoLLFileAction;
    QAction *UTMConverterLLtoUTMFileAction;
    QAction *UTMConverterDialogAction;
    QAction *LatLongConverterFileAction;
    QAction *LatLongConverterDialogAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *showHelpAction;

    QProgressDialog *ProgressDialog;
    QProgressDialog *FileProgressDialog;
    QProgressBar    *ProgressBar;
    QLabel          *StatusMessage;

    QStringListModel    *Model;
    QListView           *ListView;
    QTextEdit           *textViewer;

    QPoint	posWindow;              //!< Position des Programmfensters.
    QSize	sizeWindow;             //!< Groesse des Programmfensters.

    QPoint	posDialog;              //!< Position der Dialoge.
    QSize	sizeParameterDialog;    //!< Groesse der Parameterdialogs.
};

#endif
