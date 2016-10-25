/* 2012-05-16                 */
/* Dr. Rainer Sieger          */

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-05-16

int MainWindow::LatLongConverter( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_ColumnLatitude, const int i_ColumnLongitude, const int i_NumOfDigits, const int i_DecimalSeparator, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         n                   = 0;

    int         stopProgress        = 0;

    QString     s_EOL               = setEOLChar( i_EOL );

    QStringList sl_Input;

    convertPosition *llc = new convertPosition( i_NumOfDigits, i_DecimalSeparator );

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting positions..." ), n );

    tout << "Latitude\tLongitude\t" << sl_Input.at( i++ ) << s_EOL;

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        llc->convertLLtoLL( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ), sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) );

        tout << llc->Latitude() << "\t" << llc->Longitude() << "\t" << sl_Input.at( i ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_DeleteInputFile == true )
        removeFile( s_FilenameIn );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-05-16

void MainWindow::doLatLongConverterFile()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doLatLongOptionsDialog( gi_ll_LatitudeColumn,  gi_ll_LongitudeColumn, gi_ll_NumOfDigits, gi_ll_DecimalSeparator, gb_ll_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting positions..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = LatLongConverter( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, gi_ll_LatitudeColumn, gi_ll_LongitudeColumn, gi_ll_NumOfDigits, gi_ll_DecimalSeparator, gb_ll_DeleteInputFile, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );
        }
        else
        {
            err = _CHOOSEABORTED_;
        }
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting positions was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-05-16

void MainWindow::doLatLongConverterDialog()
{
    doLatLongDialog();
}
