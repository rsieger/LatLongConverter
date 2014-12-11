/* findArea.cpp 		    */
/* 2011-11-10              */
/* Dr. Rainer Sieger       */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/**********************************/
/*                                */
/*  R. Schlitzer (oct/21/96)      */
/*                                */
/**********************************/

int MainWindow::PtInPolygon( double d_Lat, double d_Long, int i_StartPosition, int i_NumOfPoints, QVector<PositionItem> &v_Position )
{
    int    count       = 0;
    int    npoints     = 0;

    double yf          = 0;
    double xx          = 0;
    double d_Latitude  = d_Lat;
    double d_Longitude = d_Long;

    /* make sure polygon-corners differ from test point */
    npoints = i_StartPosition + i_NumOfPoints;

    for ( int i=i_StartPosition; i<npoints; i++)
    {
        if ( ( v_Position.at( i ).Latitude() == d_Latitude ) || ( v_Position.at( i ).Longitude() == d_Longitude ) )
        {
            d_Latitude  = d_Latitude + 1.E-6*d_Latitude;
            d_Longitude = d_Longitude + 1.E-6*d_Longitude;
        }
    }

    /* check each polygon segment */
    for ( int i=i_StartPosition; i<npoints-1; i++ )
    {
        if ( v_Position.at( i ).Latitude() != v_Position.at( i+1 ).Latitude() )
        {
            yf = ( d_Latitude - v_Position.at( i ).Latitude() )/( v_Position.at( i+1 ).Latitude() - v_Position.at( i ).Latitude() );

            if ( (0. <= yf ) && ( yf <= 1. ) )
            {
                xx = v_Position.at( i ).Longitude() + yf*( v_Position.at( i+1 ).Longitude() - v_Position.at( i ).Longitude() );

                if (xx >= d_Longitude)
                    count++;
            }
        }
    }

    return (count % 2);
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::findArea( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, QVector<AreaItem> &v_Area, QVector<PositionItem> &v_Position, const bool b_DeleteInputFile, const int i_NumOfFiles)
{
    int             i                   = 1;
    int             j                   = 0;
    int             n                   = 0;
    int             m                   = 0;
    int             i_LatID             = -1;
    int             i_LongID            = -1;
    int             stopProgress        = -1;

    double          d_Latitude_dummy    = -1000.;
    double          d_Longitude_dummy   = -1000.;
    double          d_Latitude          = -1000.;
    double          d_Longitude         = -1000.;

    QString         s_AreaName          = "???";
    QString         s_EOL               = setEOLChar( i_EOL );

    QStringList     sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    m = NumOfSections( sl_Input.at( 0 ) );

    j = 0;
    while ( ( i_LatID < 0 ) && ( j < m ) )
    {
        if ( sl_Input.at( 0 ).section( "\t", j, j ).contains( "Latitude", Qt::CaseInsensitive ) == true )
            i_LatID = j;
        else
            j++;
    }

    j = 0;
    while ( ( i_LongID < 0 ) && ( j < m ) )
    {
        if ( sl_Input.at( 0 ).section( "\t", j, j ).contains( "Longitude", Qt::CaseInsensitive ) == true )
            i_LongID = j;
        else
            j++;
    }

    if ( ( i_LatID < 0 ) || ( i_LongID < 0 ) )
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

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Finding areas..." ), sl_Input.count() );

// **********************************************************************************************

    tout << "Area\t" << sl_Input.at( 0 ) << s_EOL;

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        d_Latitude  = (double) qMin( sl_Input.at( i ).section( "\t", i_LatID, i_LatID ).toFloat() + 90., 179.99 );     // 0 - 179.99
        d_Longitude = (double) qMin( sl_Input.at( i ).section( "\t", i_LongID, i_LongID ).toFloat() + 180., 359.99 );  // 0 - 359.99

        if ( ( d_Latitude-d_Latitude_dummy > 0.001 ) || ( d_Longitude-d_Longitude_dummy > 0.001 ) || ( d_Latitude-d_Latitude_dummy < -0.001 ) || ( d_Longitude-d_Longitude_dummy < -0.001 ) )
        {
            int     j               = 0;
            int     k               = 0;
            int     i_InPolygon     = 0;

            while ( j < v_Area.count() )
            {
                if ( ( v_Area.at( j ).maxLatitude() > d_Latitude ) && ( d_Latitude > v_Area.at( j ).minLatitude() ) )
                {
                    if ( ( v_Area.at( j ).maxLongitude() > d_Longitude ) && ( d_Longitude > v_Area.at( j ).minLongitude() ) )
                    {
                        i_InPolygon = PtInPolygon( d_Latitude, d_Longitude, v_Area.at( j ).StartPosition(), v_Area.at( j ).NumOfPoints(), v_Position );

                        if ( i_InPolygon == 1 )
                        {
                            s_AreaName = v_Area.at( j ).AreaName();
                            s_AreaName.replace( ", western part", "" );
                            s_AreaName.replace( ", eastern part", "" );

                            d_Latitude_dummy  = d_Latitude;
                            d_Longitude_dummy = d_Longitude;

                            tout << s_AreaName << "\t" << sl_Input.at( i ) << s_EOL;

                            j = v_Area.count() - 1;
                        }
                    }
                }

                k += v_Area.at( j ).NumOfPoints();
                j += 1;
            }

            if ( i_InPolygon < 1 )
            {
                d_Latitude_dummy  = d_Latitude;
                d_Longitude_dummy = d_Longitude;

                if ( d_Latitude > 160. )
                    s_AreaName = "Arctic Ocean";
                else
                    s_AreaName = "???";

                tout << s_AreaName << "\t" << sl_Input.at( i ) << s_EOL;
            }
        }
        else
        {
            tout << s_AreaName << "\t" << sl_Input.at( i ) << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

//--------------------------------------------------------------------------------------------

    resetProgress( i_NumOfFiles );

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
// 2011-11-05

int MainWindow::readAreaDataBaseFile( const QString &FilenameADB, QVector<AreaItem> &v_Area, QVector<PositionItem> &v_Position )
{
    int         i                   = 0;
    int         n                   = 0;
    int         stopProgress        = 0;

    int         i_NumOfAreas        = 0;
    int         i_NumOfPositions    = 0;

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( FilenameADB, sl_Input, _SYSTEM_, 1, -1 ) ) < 1 )
        return( n );

// **********************************************************************************************

    i_NumOfAreas     = sl_Input.at( 2 ).section( "\t", 1, 1 ).toInt();
    i_NumOfPositions = sl_Input.at( 3 ).section( "\t", 1, 1 ).toInt();

    for ( i=7; i<i_NumOfAreas+7; i++ )
        v_Area.append( AreaItem( sl_Input.at( i ).section( "\t", 0, 0 ), sl_Input.at( i ).section( "\t", 1, 1 ).toInt(), sl_Input.at( i ).section( "\t", 2, 2 ).toInt(), sl_Input.at( i ).section( "\t", 5, 5 ).toFloat(), sl_Input.at( i ).section( "\t", 3, 3 ).toFloat(), sl_Input.at( i ).section( "\t", 6, 6 ).toFloat(), sl_Input.at( i ).section( "\t", 4, 4 ).toFloat() ) );

    initProgress( 2, FilenameADB, tr( "Finding area database (read data) ..." ), i_NumOfPositions );

    i = i_NumOfAreas+10;
    i_NumOfPositions += i;

    while ( ( i < i_NumOfPositions ) && ( stopProgress != _APPBREAK_ ) )
    {
        v_Position.append( PositionItem( sl_Input.at( i ).section( "\t", 0, 0 ).toDouble(), sl_Input.at( i ).section( "\t", 1, 1 ).toDouble() ) );

        stopProgress = incProgress( 2, ++i );
    }

    resetProgress( 2 );

    return( i_NumOfAreas );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-10-19

int MainWindow::buildAreaDatabase( const QString& s_FilenameIn, const int i_CodecInput, const int i_NumOfFiles )
{
    int         n                   = 0;
    int         stopProgress        = 0;

    int         i                   = 0;

    int         i_NumOfAreas        = 0;
    int         i_NumOfPositions    = 0;
    int         i_StartPosition     = 0;

    int         i_AreaNamePosition  = -1;
    int         i_LatitudePosition  = -1;
    int         i_LongitudePosition = -1;

    double      d_Lat               =    0.;
    double      d_Long              =    0.;
    double      d_minLat            =  180.;
    double      d_maxLat            =    0.;
    double      d_minLong           =  360.;
    double      d_maxLong           =    0.;

    QString     s_AreaName          = "";
    QString     s_Header            = "";

    QStringList sl_Input;

    QVector<AreaItem>       v_Area;
    QVector<PositionItem>   v_Position;

// **********************************************************************************************

    QFileInfo fi( s_FilenameIn );

    QFile fout( fi.absolutePath() + "/" + fi.baseName() + ".adb" );

    if ( fout.open( QIODevice::WriteOnly ) == false )
        return( -20 );

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

    if ( sl_Input.at( i ).contains( "/*" ) == true )
    {
        while ( ( i < n ) && ( sl_Input.at( i ).contains( "*/" ) == false ) )
            i++;
    }

// **********************************************************************************************
// check file format (plain text or doi:10.1594/PANGAEA.777975)

    if ( i > 0 )
    {
        if ( n-i++ < 4 )
            return ( -40 );
    }
    else
    {
        if ( n < 4 )
            return ( -40 );
    }

    s_Header = sl_Input.at( i++ );

    for ( int j=0; j<s_Header.count(); j++ )
    {
        if ( ( s_Header.section( "\t", j, j ) == "Name" ) || ( s_Header.section( "\t", j, j ) == "Name of area" ) || ( s_Header.section( "\t", j, j ) == "Campaign" ) )
            i_AreaNamePosition = j;

        if ( s_Header.section( "\t", j, j ) == "Latitude" )
            i_LatitudePosition = j;

        if ( s_Header.section( "\t", j, j ) == "Longitude" )
            i_LongitudePosition = j;
    }

    if ( ( i_AreaNamePosition < 0 ) || ( i_LatitudePosition < 0 ) || ( i_LongitudePosition < 0 ) )
        return( -40 );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Building area database (read data) ..." ), n );

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        d_minLat	=  1000.0;
        d_maxLat	= -1000.0;
        d_minLong	=  1000.0;
        d_maxLong	= -1000.0;

        i_NumOfPositions = 0;
        i_StartPosition  = v_Position.count();
        s_AreaName       = sl_Input.at( i ).section( "\t", i_AreaNamePosition, i_AreaNamePosition );

        while ( ( i < n ) && ( ( sl_Input.at( i ).section( "\t", i_AreaNamePosition, i_AreaNamePosition ) == s_AreaName ) || ( sl_Input.at( i ).section( "\t", i_AreaNamePosition, i_AreaNamePosition ).isEmpty() == true ) ) && ( stopProgress != _APPBREAK_ ) )
        {
            i_NumOfPositions++;

            d_Lat  = (double) qMin( sl_Input.at( i ).section( "\t", i_LatitudePosition, i_LatitudePosition ).toFloat() + 90., 179.99 );
            d_Long = (double) qMin( sl_Input.at( i ).section( "\t", i_LongitudePosition, i_LongitudePosition ).toFloat() + 180., 359.99 );

            v_Position.append( PositionItem( d_Lat, d_Long ) );

            d_minLat  = qMin( d_minLat, v_Position.last().Latitude() );
            d_maxLat  = qMax( d_maxLat, v_Position.last().Latitude() );
            d_minLong = qMin( d_minLong, v_Position.last().Longitude() );
            d_maxLong = qMax( d_maxLong, v_Position.last().Longitude() );

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        v_Area.append( AreaItem( s_AreaName, i_StartPosition, i_NumOfPositions, d_minLat, d_maxLat, d_minLong, d_maxLong ) );
    }

    resetProgress( i_NumOfFiles );

//----------------------------------------------------------------------------------------------------

    if ( stopProgress != _APPBREAK_ )
    {
        i_NumOfAreas		= v_Area.count();
        i_NumOfPositions	= v_Position.count();

        QByteArray ba( "PanTool area database\n" );                                                             // 0

        ba.append( QString( "Created:\t%1\n" ).arg( QDateTime::currentDateTime().toString( Qt::ISODate ) ) );   // 1
        ba.append( QString( "Number of areas:\t%1\n" ).arg( i_NumOfAreas ) );                                   // 2
        ba.append( QString( "Number of positions:\t%1\n" ).arg( i_NumOfPositions ) );                           // 3

        ba.append( "\n[Areas]\n" );
        ba.append( "Name of area\tStart position\tNumber of positions\tmax Lat\tmax Long\tmin Lat\tmin Long\n" );               // 4

        for ( i=0; i<i_NumOfAreas; i++ )
        {
            ba.append( v_Area.at( i ).AreaName() + "\t" );
            ba.append( QString( "%1\t%2\t%3\t%4\t%5\t%6\n" ).arg( v_Area.at( i ).StartPosition() ).arg( v_Area.at( i ).NumOfPoints() ).arg( v_Area.at( i ).maxLatitude() ).arg( v_Area.at( i ).maxLongitude() ).arg( v_Area.at( i ).minLatitude() ).arg( v_Area.at( i ).minLongitude() ) );
        }

        initProgress( i_NumOfFiles, s_FilenameIn, tr( "Building area database (write data) ..." ), i_NumOfPositions );

        i = 0;

        ba.append( "\n[Positions]\n" );
        ba.append( "Latitude\tLongitude\n" );

        while ( ( i<i_NumOfPositions ) && ( stopProgress != _APPBREAK_ ) )
        {
            ba.append( QString( "%1\t%2\n" ).arg( v_Position.at( i ).Latitude(), 0, 'f', 6 ).arg( v_Position.at( i ).Longitude(), 0, 'f', 6 ) );
            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        resetProgress( i_NumOfFiles );

        fout.write( qCompress( ba, 9 ) );

        fout.close();
    }

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
/*
int MainWindow::WriteADB( const QString& FilenameADB )
{
    QString FilenameOut = FilenameADB;
    FilenameOut.replace( "//", "/" );

    QFile fout( FilenameOut );
    if ( !fout.open( IO_WriteOnly | IO_Translate) )
        return( -20 );

    QTextStream tout( &fout ); // tout.setEncoding( QTextStream::Latin1 );

    tout << "Latitude\tLongitude\tArea" << s_EOL;
    tout << "AreaID\t4\tName of area" << s_EOL;
    tout << "76.449997\t-78.466667" << s_EOL;
    tout << "76.099998\t-78.816673" << s_EOL;
    tout << "75.841667\t-78.916672" << s_EOL;
    tout << "76.449997\t-78.466667" << s_EOL;
    tout << "AreaID\t7\tName of area" << s_EOL;
    tout << "69.599998\t60.200001" << s_EOL;
    tout << "69.000000\t63.000000" << s_EOL;
    tout << "67.000000\t55.000000" << s_EOL;
    tout << "67.000000\t50.000000" << s_EOL;
    tout << "66.000000\t45.000000" << s_EOL;
    tout << "68.000000\t45.000000" << s_EOL;
    tout << "69.599998\t60.200001" << s_EOL;

    fout.close();

    return( _NOERROR_ );
}
*/
// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 12.03.2006

void MainWindow::doFindArea()
{
    int             i               = 0;
    int             err             = 0;
    int             stopProgress    = 0;

    QString         s_FilenameIn    = "";
    QString         s_FilenameOut   = "";

    QVector<AreaItem>       v_Area;
    QVector<PositionItem>   v_Position;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doFindAreaDialog( gs_AreaDatabaseFilename, gb_fa_DeleteInputFile ) == QDialog::Accepted )
        {
            if ( readAreaDataBaseFile( gs_AreaDatabaseFilename, v_Area, v_Position ) )
            {
                initFileProgress( gsl_FilenameList.count()+1, gsl_FilenameList.at( 0 ), tr( "Finding area ..." ) );

                stopProgress = incFileProgress( gsl_FilenameList.count()+1, 1 );

                while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
                {

                    if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( 0 ), s_FilenameIn, s_FilenameOut ) == true )
                    {
                        err = findArea( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_Extension, v_Area, v_Position, gb_fa_DeleteInputFile, gsl_FilenameList.count()+1 );

                        stopProgress = incFileProgress( gsl_FilenameList.count()+1, ++i+1 );
                    }
                    else
                    {
                        err = _FILENOTEXISTS_;
                    }
                }

                resetFileProgress( gsl_FilenameList.count()+1 );
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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Finding area was canceled" ), true, false );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 22.03.2004

void MainWindow::doBuildAreaDatabase()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Building area database ..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = buildAreaDatabase( s_FilenameIn, gi_CodecInput, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Building of area database was canceled" ), true, false );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

AreaItem::AreaItem( const QString AreaName, const int StartPosition, const int NumOfPoints, const float minLatitude, const float maxLatitude, const float minLongitude, const float maxLongitude )
{
    setAreaName( AreaName );
    setStartPosition( StartPosition );
    setNumOfPoints( NumOfPoints );
    setMinLatitude( minLatitude);
    setMaxLatitude( maxLatitude );
    setMinLongitude( minLongitude );
    setMaxLongitude( maxLongitude );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

PositionItem::PositionItem( const float Latitude, const float Longitude )
{
    setLatitude( Latitude );
    setLongitude( Longitude );
}
