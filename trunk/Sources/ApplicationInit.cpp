/* ApplicationMainWindow.cpp  */
/* 2009-08-09                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Initalisierung des CentralWidget.
*
*   Am Beginn der Methode wird der Cursor auf den WaitCursor des Betriebssystems
*   gesetzt. Dann folgt die Intialisierung einiger globaler Variablen (ActionNumer, sl_FilenameList,
*   sl_Message). Model und ListView werden erzeugt und das CentralWidget gesetzt. Nach dem Laden
*   der Programmeinstellungen werden die Menues erzeugt. Die Fensterposition und -groesse wird
*   gesetzt. Abschliessend wird das Fenster angezeigt und der Cursor zuruek auf den NormalCursor gesetzt.
*/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
//  QObject::connect( this, SIGNAL( finishedBuildFilelist( bool ) ), this, SLOT( doIt( bool ) ) );

// **********************************************************************************************

    setWaitCursor();

// **********************************************************************************************
// init

    gi_ActionNumber                         = 1;                 //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.

    gs_Version                              = "unknown";         //!< Aktuelle Version des Programms.

    gsl_FilenameList.clear();

// **********************************************************************************************

    Model       = new QStringListModel( this );
    ListView    = new QListView;

    ListView->setModel( Model );
    ListView->setEditTriggers( QAbstractItemView::NoEditTriggers );

    setCentralWidget( ListView );

    setAcceptDrops( true );

// **********************************************************************************************

    loadPreferences();

    if ( gi_NumOfProgramStarts++ < 1 )
        savePreferences();

    gs_Version = getVersion();

// **********************************************************************************************

    createActions();
    createMenus();
    createStatusBar();

// **********************************************************************************************

    this->resize( sizeWindow );
    this->move( posWindow );
    this->show();

// **********************************************************************************************

    gsl_FilenameList = expandCommandline();

// **********************************************************************************************

    setWTitle( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );
    setStatusBar( tr( "Ready" ), 2 );
    setNormalCursor();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

MainWindow::~MainWindow()
{
}
