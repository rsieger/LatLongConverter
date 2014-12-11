/* 2012-08-13                 */
/* Dr. Rainer Sieger          */

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-05-16

int MainWindow::UTMConverter( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                              const int i_ColumnLatitude, const int i_ColumnLongitude, const int i_ColumnNorthing, const int i_ColumnEasting, const int i_ColumnZone,
                              const int i_ReferenceEllipsoid, const int i_NumOfDigits, const int i_DecimalSeparator, const bool b_DeleteInputFile, const int i_NumOfFiles )
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

    if ( ( i_ColumnNorthing > 0 ) && ( i_ColumnEasting > 0 ) && ( i_ColumnZone > 0 ) )
        mode = _UTMLL_;

    if ( ( i_ColumnLatitude > 0 ) && ( i_ColumnLongitude > 0 ) )
        mode = _LLUTM_;

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
    case _UTMLL_:
        tout << sl_Input.at( i++ ) << "\tLatitude\tLongitude" << s_EOL;
        break;
    case _LLUTM_:
        tout << sl_Input.at( i++ ) << "\tNorthing [m]\tEasting [m]\tZone" << s_EOL;
        break;
    }

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        switch( mode )
        {
        case _UTMLL_:
            llc->convertUTMtoLL( sl_Input.at( i ).section( "\t", i_ColumnZone-1, i_ColumnZone-1 ), sl_Input.at( i ).section( "\t", i_ColumnEasting-1, i_ColumnEasting-1 ), sl_Input.at( i ).section( "\t", i_ColumnNorthing-1, i_ColumnNorthing-1 ), i_ReferenceEllipsoid );
            tout << sl_Input.at( i ) << "\t" << llc->Latitude() << "\t" << llc->Longitude() << s_EOL;
            break;

        case _LLUTM_:
            if ( ( llc->getMode( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ) ) == 1 ) || ( llc->getMode( sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) ) == 1 ) )
            {
                llc->setNumOfDigitsLL( 7 );
                llc->convertLLtoLL( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ), sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ) );
                llc->convertLLtoUTM( llc->Latitude(), llc->Longitude(), i_ReferenceEllipsoid );
            }
            else
            {
                llc->convertLLtoUTM( sl_Input.at( i ).section( "\t", i_ColumnLatitude-1, i_ColumnLatitude-1 ), sl_Input.at( i ).section( "\t", i_ColumnLongitude-1, i_ColumnLongitude-1 ), i_ReferenceEllipsoid );
            }

            tout << sl_Input.at( i ) << "\t" << QString( "%1" ).arg( llc->Northing(), 0, 'f', i_NumOfDigits ) << "\t" << QString( "%1" ).arg( llc->Easting(), 0, 'f', i_NumOfDigits ) << "\t" << llc->Zone() << s_EOL;

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

void MainWindow::doLLtoUTMConverterFile()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doUTMOptionsDialog( _LLUTM_, gi_utm_LatitudeColumn, gi_utm_LongitudeColumn, gi_utm_NorthingColumn, gi_utm_EastingColumn, gi_utm_ZoneColumn, gi_utm_ReferenceEllipsoid, gi_utm_NumOfDigits_UTM, gi_utm_DecimalSeparator, gb_utm_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting positions..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = UTMConverter( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, gi_utm_LatitudeColumn, gi_utm_LongitudeColumn, -1, -1, -1, gi_utm_ReferenceEllipsoid, gi_utm_NumOfDigits_UTM, gi_utm_DecimalSeparator, gb_utm_DeleteInputFile, gsl_FilenameList.count() );

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

void MainWindow::doUTMtoLLConverterFile()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doUTMOptionsDialog( _UTMLL_, gi_utm_LatitudeColumn, gi_utm_LongitudeColumn, gi_utm_NorthingColumn, gi_utm_EastingColumn, gi_utm_ZoneColumn, gi_utm_ReferenceEllipsoid, gi_utm_NumOfDigits_LL, gi_utm_DecimalSeparator, gb_utm_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting positions..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = UTMConverter( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, -1, -1, gi_utm_NorthingColumn, gi_utm_EastingColumn, gi_utm_ZoneColumn, gi_utm_ReferenceEllipsoid, gi_utm_NumOfDigits_LL, gi_utm_DecimalSeparator, gb_utm_DeleteInputFile, gsl_FilenameList.count() );

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

void MainWindow::doUTMConverterDialog()
{
    doUTMDialog();
}
