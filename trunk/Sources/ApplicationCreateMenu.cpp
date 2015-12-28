// ***********************************************************************************************
// *                                                                                             *
// * createMenu.cpp - creates application menus                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2008-05-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erstellen der Menue-Aktionen. */

void MainWindow::createActions()
{
// File menu
    newWindowAction = new QAction(tr("&New window"), this);
    newWindowAction->setShortcut(tr("Ctrl+N"));
    connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(chooseFiles()));

    openFolderAction = new QAction(tr("Select &folder..."), this);
    openFolderAction->setShortcut(tr("Ctrl+F"));
    connect(openFolderAction, SIGNAL(triggered()), this, SLOT(chooseFolder()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("Save &as..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    hideWindowAction = new QAction(tr("&Close window"), this);
    hideWindowAction->setShortcut(tr("Ctrl+W"));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hideWindow()));

    setOptionsAction = new QAction(tr("General options..."), this);
    setOptionsAction->setShortcut(tr("Ctrl+,"));
    connect(setOptionsAction, SIGNAL(triggered()), this, SLOT(doGeneralOptionsDialog()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Basic tools

    LatLongConverterFileAction = new QAction(tr("Convert Latitude/Longitude (Batch)..."), this);
    connect(LatLongConverterFileAction, SIGNAL(triggered()), this, SLOT(doLatLongConverterFile()));

    LatLongConverterDialogAction = new QAction(tr("Convert Latitude/Longitude (Single position)..."), this);
    connect(LatLongConverterDialogAction, SIGNAL(triggered()), this, SLOT(doLatLongConverterDialog()));

    UTMConverterLLtoUTMFileAction = new QAction(tr("Convert Latitude/Longitude to UTM coordinates (Batch)..."), this);
    connect(UTMConverterLLtoUTMFileAction, SIGNAL(triggered()), this, SLOT(doLLtoUTMConverterFile()));

    UTMConverterUTMtoLLFileAction = new QAction(tr("Convert UTM coordinates to Latitude/Longitude (Batch)..."), this);
    connect(UTMConverterUTMtoLLFileAction, SIGNAL(triggered()), this, SLOT(doUTMtoLLConverterFile()));

    UTMConverterDialogAction = new QAction(tr("Convert UTM coordinates (Single position)..."), this);
    connect(UTMConverterDialogAction, SIGNAL(triggered()), this, SLOT(doUTMConverterDialog()));

    GaussKruegerConverterLLtoGKFileAction = new QAction(tr("Convert Latitude/Longitude to Gauss-Krüger coordinates (Batch)..."), this);
    connect(GaussKruegerConverterLLtoGKFileAction, SIGNAL(triggered()), this, SLOT(doLLtoGKConverterFile()));

    GaussKruegerConverterGKtoLLFileAction = new QAction(tr("Convert Gauss-Krüger coordinates to Latitude/Longitude (Batch)..."), this);
    connect(GaussKruegerConverterGKtoLLFileAction, SIGNAL(triggered()), this, SLOT(doGKtoLLConverterFile()));

    GaussKruegerConverterDialogAction = new QAction(tr("Convert Gauss-Krüger coordinates (Single position)..."), this);
    connect(GaussKruegerConverterDialogAction, SIGNAL(triggered()), this, SLOT(doGKConverterDialog()));

    findPolygonCentroidAction = new QAction(tr("Find area centroid"), this);
    connect(findPolygonCentroidAction, SIGNAL(triggered()), this, SLOT(doFindPolygonCentroid()));

    findAreaAction = new QAction(tr("Find area"), this);
    connect(findAreaAction, SIGNAL(triggered()), this, SLOT(doFindArea()));

    buildAreaDatabaseAction = new QAction(tr("Build area database"), this);
    connect(buildAreaDatabaseAction, SIGNAL(triggered()), this, SLOT(doBuildAreaDatabase()));

    // Help menu
    aboutAction = new QAction(tr("&About ") + getApplicationName( true ), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    showHelpAction = new QAction(getApplicationName( true ) + tr(" &Help"), this);
    showHelpAction->setShortcut(tr("F1"));
    connect(showHelpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

#if defined(Q_OS_WIN)
    newWindowAction->setStatusTip(tr("Create a new file"));
    openFileAction->setStatusTip(tr("Choose an existing file"));
    openFolderAction->setStatusTip(tr("Choose an existing folder"));
    saveAction->setStatusTip(tr("Save the document to disk"));
    saveAsAction->setStatusTip(tr("Save the document under a new name"));
    exitAction->setStatusTip(tr("Exit the application"));
    aboutAction->setStatusTip(tr("Show the application's About box"));
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    showHelpAction->setStatusTip(tr("Show the application's help"));
#endif

}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Verbindet Menues mit Aktionen. */

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( openFileAction );
    fileMenu->addAction( openFolderAction );
    fileMenu->addSeparator();

#if defined(Q_OS_LINUX)
    ;
#endif

#if defined(Q_OS_MAC)
    fileMenu->addAction( newWindowAction );
    newWindowAction->setEnabled( false );
    fileMenu->addAction( hideWindowAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addAction( hideWindowAction );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( setOptionsAction );

#if defined(Q_OS_LINUX)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

// **********************************************************************************************

    specialToolsMenu = menuBar()->addMenu( tr( "Converter" ) );

    specialToolsMenu->addAction( LatLongConverterFileAction );
    specialToolsMenu->addAction( LatLongConverterDialogAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( UTMConverterLLtoUTMFileAction );
    specialToolsMenu->addAction( UTMConverterUTMtoLLFileAction );
    specialToolsMenu->addAction( UTMConverterDialogAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( GaussKruegerConverterLLtoGKFileAction );
    specialToolsMenu->addAction( GaussKruegerConverterGKtoLLFileAction );
    specialToolsMenu->addAction( GaussKruegerConverterDialogAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( findAreaAction );
    specialToolsMenu->addAction( findPolygonCentroidAction );
    specialToolsMenu->addAction( buildAreaDatabaseAction );

// **********************************************************************************************

    helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
    helpMenu->addSeparator();
    helpMenu->addAction( showHelpAction );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::enableMenuItems( const QStringList sl_FilenameList )
{
    bool b_containsBinaryFile = containsBinaryFile( sl_FilenameList );

// **********************************************************************************************

    QList<QAction*> specialToolsMenuActions = specialToolsMenu->actions();

    if ( b_containsBinaryFile == false )
    {
        for ( int i=0; i<specialToolsMenuActions.count(); ++i )
            specialToolsMenuActions.at( i )->setEnabled( true );
    }
    else
    {
        for ( int i=0; i<specialToolsMenuActions.count(); ++i )
            specialToolsMenuActions.at( i )->setEnabled( false );
    }
}
