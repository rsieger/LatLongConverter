// ***********************************************************************************************
// *                                                                                             *
// * FindAreaDialog.cpp - set area database file                                                 *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-12-12                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "FindAreaDialog.h"

FindAreaDialog::FindAreaDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(BrowseAreaDatabase_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
    connect(AreaDatabase_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT(enableOKButton()));
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FindAreaDialog::enableOKButton()
{
    bool b_OK = true;

    QFileInfo fiADB( AreaDatabase_lineEdit->text() );

    if ( ( fiADB.isFile() == false ) || ( fiADB.exists() == false ) )
    {
        AreaDatabase_lineEdit->setText( tr( "Browse through the area database with \"Browse ...\"" ) );
        b_OK = false;
    }

    if ( b_OK == true )
    {
        OK_pushButton->setEnabled( true );
        OK_pushButton->setDefault( true );
    }
    else
    {
        OK_pushButton->setEnabled( false );
        Cancel_pushButton->setDefault( true );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doFindAreaDialog( QString &s_AreaDatabaseFilename, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    FindAreaDialog dialog( this );

    dialog.AreaDatabase_lineEdit->setText( QDir::toNativeSeparators( s_AreaDatabaseFilename ) );
    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.enableOKButton();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        s_AreaDatabaseFilename = dialog.AreaDatabase_lineEdit->text();
        b_DeleteInputFile      = dialog.DeleteInputFile_checkBox->isChecked();

        i_DialogResult         = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void FindAreaDialog::browseFilenameDialog()
{
    QString	fn   = "";
    QString file = AreaDatabase_lineEdit->text();

    QFileInfo fi( file );

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        file = getDocumentDir();

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the area database file (*.adb)" ), file, tr( "ADB file (*.adb)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the area database file (*.adb)" ), file, tr( "ADB file (*.adb)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the area database file (*.adb)" ), file, tr( "ADB file (*.adb)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == false )
        fi.setFile( fn );
    else
        fn = file;

    if ( ( fi.isFile() == false ) || ( fi.exists() == false ) )
        AreaDatabase_lineEdit->clear();
    else
        AreaDatabase_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    AreaDatabase_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString FindAreaDialog::getDocumentDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_WIN)
        return( QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) );
    #endif
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FindAreaDialog::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat( "text/uri-list" ) )
        event->acceptProposedAction();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void FindAreaDialog::dropEvent( QDropEvent *event )
{
    QList<QUrl> urls = event->mimeData()->urls();

    if ( urls.isEmpty() == true )
        return;

    QString s_fileName = urls.first().toLocalFile();

    if ( s_fileName.isEmpty() == true )
        return;

    QFileInfo fi( s_fileName );

    if ( fi.isFile() == true )
    {
        if ( fi.suffix().toLower() == "adb" )
            AreaDatabase_lineEdit->setText( QDir::toNativeSeparators( s_fileName ) );
    }
}
