/*
 * ANSI C code from the article
 * "Centroid of a Polygon"
 * by Gerard Bashein and Paul R. Detmer,
    (gb@locke.hs.washington.edu, pdetmer@u.washington.edu)
 * in "Graphics Gems IV", Academic Press, 1994
 *
 * added to PanTool by Rainer Sieger, Alfred Wegener Institute Bremerhaven
 * 2012-03-14
 *
 */

/*********************************************************************

polyCentroid: Calculates the centroid (xCentroid, yCentroid) and area
of a polygon, given its vertices (x[0], y[0]) ... (x[n-1], y[n-1]). It
is assumed that the contour is closed, i.e., that the vertex following
(x[n-1], y[n-1]) is (x[0], y[0]).  The algebraic sign of the area is
positive for counterclockwise ordering of vertices in x-y plane;
otherwise negative.

Returned values:  0 for normal execution;  1 if the polygon is
degenerate (number of vertices < 3);  and 2 if area = 0 (and the
centroid is undefined).

**********************************************************************/

#include "Application.h"

int MainWindow::polyCentroid( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
     int    stopProgress        = 0;
     int    err                 = 0;

     int    i                   = 0;
     int    j                   = 0;
     int    k                   = 0;
     int    n                   = 0;

     int    i_NumOfPositions    = 0;
     int    i_StartPosition     = 0;

     int    i_AreaNamePosition  = -1;
     int    i_LatitudePosition  = -1;
     int    i_LongitudePosition = -1;
     int    i_AreaIDPosition    = -1;

     double x[1000];
     double y[1000];
     double xCentroid    = 0.;
     double yCentroid    = 0.;
     double area         = 0.;

     double ai           = 0.;
     double atmp         = 0.;
     double xtmp         = 0.;
     double ytmp         = 0.;

     QString s_EOL       = setEOLChar( i_EOL );
     QString s_AreaName  = "";
     QString s_Header    = "";

     QStringList    sl_Input;

// **********************************************************************************************
// read file

     if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
         return( n );

     if ( sl_Input.at( k ).contains( "/*" ) == true )
     {
         while ( ( k < n ) && ( sl_Input.at( k ).contains( "*/" ) == false ) )
             k++;
     }

// **********************************************************************************************
// check file format (plain text or doi:10.1594/PANGAEA.777975)

     if ( n-k++ < 4 )
         return ( -40 );

     s_Header = sl_Input.at( k++ );

     for ( i=0; i<s_Header.count(); i++ )
     {
         if ( ( s_Header.section( "\t", i, i ) == "Name" ) || ( s_Header.section( "\t", i, i ) == "Name of area" ) || ( s_Header.section( "\t", i, i ) == "Campaign" ) )
             i_AreaNamePosition = i;

         if ( s_Header.section( "\t", i, i ) == "Latitude" )
             i_LatitudePosition = i;

         if ( s_Header.section( "\t", i, i ) == "Longitude" )
             i_LongitudePosition = i;

         if ( s_Header.section( "\t", i, i ) == "Index (of ocean or sea)" )
             i_AreaIDPosition = i;
     }

     if ( ( i_AreaNamePosition < 0 ) || ( i_LatitudePosition < 0 ) || ( i_LongitudePosition < 0 ) )
         return( -40 );

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

     tout << "Name of area\tLatitude\tLongitude\tArea";

     if ( i_AreaIDPosition > -1 )
         tout << "\tArea ID";

     tout << s_EOL;

// **********************************************************************************************

     initProgress( i_NumOfFiles, s_FilenameIn, tr( "Find area centroid..." ), sl_Input.count() );

     while ( ( k < n ) && ( stopProgress != _APPBREAK_ ) )
     {
         i_NumOfPositions = 0;
         i_StartPosition  = k;
         s_AreaName       = sl_Input.at( k ).section( "\t", i_AreaNamePosition, i_AreaNamePosition );

         while ( ( k < n ) && ( ( sl_Input.at( k ).section( "\t", i_AreaNamePosition, i_AreaNamePosition ) == s_AreaName ) || ( sl_Input.at( k ).section( "\t", i_AreaNamePosition, i_AreaNamePosition ).isEmpty() == true ) ) )
         {
            i_NumOfPositions++;
            k++;
         }

         for ( i=0; i<i_NumOfPositions; i++ )
         {
             x[i] = sl_Input.at( i_StartPosition + i ).section( "\t", i_LatitudePosition, i_LatitudePosition ).toDouble();
             y[i] = sl_Input.at( i_StartPosition + i ).section( "\t", i_LongitudePosition, i_LongitudePosition ).toDouble();
         }

         atmp = 0.; xtmp = 0; ytmp = 0.;

         for ( i = i_NumOfPositions-1, j = 0; j < i_NumOfPositions; i = j, j++ )
         {
            ai = x[i] * y[j] - x[j] * y[i];
            atmp += ai;
            xtmp += (x[j] + x[i]) * ai;
            ytmp += (y[j] + y[i]) * ai;
         }

         area = atmp / 2.;
         err  = 2;

         if ( atmp != 0. )
         {
            xCentroid =	xtmp / (3. * atmp);
            yCentroid =	ytmp / (3. * atmp);

            tout << sl_Input.at( i_StartPosition ).section( "\t", i_AreaNamePosition, i_AreaNamePosition ) << "\t" << xCentroid << "\t" << yCentroid << "\t" << area;

            if ( i_AreaIDPosition > -1 )
                tout << "\t" << sl_Input.at( i_StartPosition ).section( "\t", i_AreaIDPosition, i_AreaIDPosition );

            tout << s_EOL;

            err = 0;
         }

         stopProgress = incProgress( i_NumOfFiles, k );
     }

     resetProgress( i_NumOfFiles );

// **********************************************************************************************

     fout.close();

     if ( stopProgress == _APPBREAK_ )
         return( _APPBREAK_ );

     if ( b_DeleteInputFile == true )
         removeFile( s_FilenameIn );

     return( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-03-14

void MainWindow::doFindPolygonCentroid()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Find polygon centroid ..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = polyCentroid( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, false, gsl_FilenameList.count() );

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

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Finding polygon centroid was canceled" ) );

    onError( err );
}

