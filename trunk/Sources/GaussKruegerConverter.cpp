/* 2012-08-13                 */
/* Dr. Rainer Sieger          */

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-05-16

int MainWindow::GaussKruegerConverter( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_ColumnLatitude, const int i_ColumnLongitude, const int i_ColumnHoch, const int i_ColumnRechts, const int i_ColumnStreifen, const int i_NumOfDigits, const int i_DecimalSeparator, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         n                   = 0;
    int         mode                = -1;

    int         stopProgress        = 0;

    QString     s_EOL               = setEOLChar( i_EOL );

    QStringList sl_Input;

    convertPosition *llc = new convertPosition( i_NumOfDigits, i_DecimalSeparator );

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************

    if ( ( i_ColumnHoch > 0 ) && ( i_ColumnRechts > 0 ) && ( i_ColumnStreifen > 0 ) )
        mode = _GKLL_;

    if ( ( i_ColumnLatitude > 0 ) && ( i_ColumnLongitude > 0 ) )
        mode = _LLGK_;

    if ( mode < 0 )
        return( mode );

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

    switch( mode )
    {
    case _GKLL_:
        tout << "Latitude\tLongitude\t" << sl_Input.at( i++ ) << s_EOL;
        break;
    case _LLGK_:
        tout << "Hoch [m]\tRechts [m]\tStreifen\t" << sl_Input.at( i++ ) << s_EOL;
        break;
    }

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        switch( mode )
        {
        case _GKLL_:
            llc->convertGKtoLL( sl_Input.at( i ).section( "\t", i_ColumnStreifen-1, i_ColumnStreifen-1 ), sl_Input.at( i ).section( "\t", i_ColumnRechts-1, i_ColumnRechts-1 ), sl_Input.at( i ).section( "\t", i_ColumnHoch-1, i_ColumnHoch-1 ) );
            tout << sl_Input.at( i ) << "\t" << llc->Latitude() << "\t" << llc->Longitude() << s_EOL;
            break;

        case _LLGK_:
            if ( ( llc->getMode( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ) ) == 1 ) || ( llc->getMode( sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) ) == 1 ) )
            {
                llc->setNumOfDigitsLL( 7 );
                llc->convertLLtoLL( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ), sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) );
                llc->convertLLtoGK( llc->Latitude(), llc->Longitude() );
            }
            else
            {
                llc->convertLLtoGK( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ), sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) );
            }

            tout << QString( "%1" ).arg( llc->Hoch(), 0, 'f', i_NumOfDigits ) << "\t" << QString( "%1" ).arg( llc->Rechts(), 0, 'f', i_NumOfDigits ) << "\t" << llc->Streifen() << "\t" << sl_Input.at( i ) << s_EOL;

            break;
        }

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

void MainWindow::doLLtoGKConverterFile()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doGaussKruegerOptionsDialog( _LLGK_, gi_gk_LatitudeColumn, gi_gk_LongitudeColumn, gi_gk_HochColumn, gi_gk_RechtsColumn, gi_gk_StreifenColumn, gi_gk_NumOfDigits_GK, gi_gk_DecimalSeparator, gb_gk_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting positions..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = GaussKruegerConverter( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, gi_gk_LatitudeColumn, gi_gk_LongitudeColumn, -1, -1, -1, gi_gk_NumOfDigits_GK, gi_gk_DecimalSeparator, gb_gk_DeleteInputFile, gsl_FilenameList.count() );

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

void MainWindow::doGKtoLLConverterFile()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doGaussKruegerOptionsDialog( _GKLL_, gi_gk_LatitudeColumn, gi_gk_LongitudeColumn, gi_gk_HochColumn, gi_gk_RechtsColumn, gi_gk_StreifenColumn, gi_gk_NumOfDigits_LL, gi_gk_DecimalSeparator, gb_gk_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting positions..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = GaussKruegerConverter( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, -1, -1, gi_gk_HochColumn, gi_gk_RechtsColumn, gi_gk_StreifenColumn, gi_gk_NumOfDigits_LL, gi_gk_DecimalSeparator, gb_gk_DeleteInputFile, gsl_FilenameList.count() );

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

void MainWindow::doGKConverterDialog()
{
    doGaussKruegerDialog();
}
