/* ApplicationErrors.cpp      */
/* 2008-03-27                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! Errorhandling */

void MainWindow::onError( const int err )
{
    switch ( err )
    {
    case _APPBREAK_:  // Progress aborted
        break;
    case _ERROR_:     // Error
        break;
    case _NOERROR_:   // No error
        break;
    case _CHOOSEABORTED_: // Choose aborted
        break;
    case _FILENOTEXISTS_: // File not exists
        QMessageBox::information( this, getApplicationName( true ), tr( "Import file not exists." ) );
        break;
    case _WRONGTIMEFORMAT_: // Time format wrong
        QMessageBox::information( this, getApplicationName( true ), tr( "Format of time was wrong.\nMust be hh:mm:ss or hh:mm:ss.sss." ) );
        break;
    case -3: // Progress aborted
        break;
    case -10:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't open import file.\nPossible locked by another application." ) );
        break;
    case -20:
        QMessageBox::information( this, getApplicationName( true ), tr( "Can't create export file.\nFile is already open." ) );
        break ;
    case -30:
//      QMessageBox::information( this, getApplicationName( true ), tr( "Converter was canceled" ) );
        break ;
    case -40:
        QMessageBox::information( this, getApplicationName( true ), tr( "Wrong format" ) );
        break ;
    case -50:
        QMessageBox::information( this, getApplicationName( true ), tr( "Only one file selected" ) );
        break ;
    case -60:
        QMessageBox::information( this, getApplicationName( true ), tr( "No parameter selected" ) );
        break ;
    case -70:
        QMessageBox::information( this, getApplicationName( true ), tr( "Number of lines is different.\nConcatenating files was canceled" ) );
        break ;
    case -80:
        QMessageBox::information( this, getApplicationName( true ), tr( "First line is empty. Can't find a column." ) );
        break ;
    case -81:
//      QMessageBox::information( this, getApplicationName( true ), tr( "Empty output file" ) );
        break ;
    case -82:
        QMessageBox::information( this, getApplicationName( true ), tr( "No match!\nEmpty output file deleted." ) );
        break ;
    case -83:
        QMessageBox::information( this, getApplicationName( true ), tr( "No match!" ) );
        break ;
    case -84:
        QMessageBox::information( this, getApplicationName( true ), tr( "Number of files will be greater than 10000.\nThis is too much." ) );
        break ;
    case -85:
        QMessageBox::information( this, getApplicationName( true ), tr( "Not enough memory." ) );
        break ;

    default :
        QMessageBox::information( this, getApplicationName( true ), tr( "Unknown error.\nPlease contact rsieger@pangaea.de" ) );
        break ;
    }
}

