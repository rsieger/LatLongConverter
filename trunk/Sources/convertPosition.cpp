/* 2012-07-29                 */
/* Dr. Rainer Sieger          */

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

convertPosition::convertPosition( const int i_NumOfDigits, const int i_DecimalSeparator)
{
    setNumOfDigitsLL( i_NumOfDigits );
    setNumOfDigitsGK( i_NumOfDigits );
    setDecimalSeparator( i_DecimalSeparator );

    initReferenceEllipsoid( v_Ellipsoid );

    return;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*Reference ellipsoids derived from Peter H. Dana's website-
http://www.utexas.edu/depts/grg/gcraft/notes/datum/elist.html
Department of Geography, University of Texas at Austin
Internet: pdana@mail.utexas.edu
3/22/95

Source
Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to Department of Defense World Geodetic System
1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency

Converts UTM coords to lat/long. Equations from USGS Bulletin 1532 (hdl:10013/epic.46739.d001)
East Longitudes are positive, West longitudes are negative.
North latitudes are positive, South latitudes are negative
Lat and Long are in decimal degrees.
Written by Chuck Gantz - chuck.gantz@globalstar.com
rewritten for Qt/C++ by Rainer Sieger - rsieger@pangaea.de
*/

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int convertPosition::convertUTMtoLL( const QString &s_Zone, const QString &s_Easting, const QString &s_Northing, const int i_ReferenceEllipsoid )
{
    QString sd_Easting  = s_Easting;
    QString sd_Northing = s_Northing;

    sd_Easting.replace( ",", "." );
    sd_Northing.replace( ",", "." );

    zone        = s_Zone;
    easting     = sd_Easting.toDouble();
    northing    = sd_Northing.toDouble();

// **********************************************************************************************

    int	   err              = _NOERROR_;

    double k0				= 0.9996;
    double a				= v_Ellipsoid.at( i_ReferenceEllipsoid ).EquatorialRadius();
    double eccSquared		= v_Ellipsoid.at( i_ReferenceEllipsoid ).EccentricitySquared();
    double eccPrimeSquared	= (eccSquared)/(1.-eccSquared);
    double e1				= (1.-sqrt(1.-eccSquared))/(1.+sqrt(1.-eccSquared));
    double N1, T1, C1, R1;
    double D;
    double mu;
    double phi1Rad;
//  double phi1;

    double dLatitude        = 0.;
    double dLongitude       = 0.;

    double dUTMEasting		= easting;
    double dUTMNorthing		= northing;

    double PI				= 3.14159265;
    double rad2deg			= 180./PI;

    int ZoneNumber			= -1;
    int ZoneLetter			= -1;

/*****************************************************************************/

    if ( getZone( s_Zone, ZoneNumber, ZoneLetter ) < 0 )
        err = -1;

    if ( ( ZoneLetter < 78 ) && ( ( dUTMNorthing < 1118385 ) || ( dUTMNorthing > 9999999 ) ) )
        err = -2;

    if ( ( ZoneLetter >= 78 ) && ( ( dUTMNorthing < 0 ) || ( dUTMNorthing > 9328112 ) ) )
        err = -3;

    if ( ( dUTMEasting < 161622 ) || ( dUTMEasting > 999999 ) ) // 778266, 809999
        err = -4;

    if ( err != _NOERROR_ )
    {
        latitude  = "-999.999";
        longitude = "-999.999";
        return( err );
    }

/*****************************************************************************/

    dUTMEasting -= 500000.0; //remove 500,000 meter offset for longitude

    if ( ZoneLetter < 78 )
        dUTMNorthing -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere

    mu          = (dUTMNorthing/k0)/(a*(1.-eccSquared/4.-3.*eccSquared*eccSquared/64.-5.*eccSquared*eccSquared*eccSquared/256.));
    phi1Rad     = mu+(3.*e1/2.-27.*e1*e1*e1/32.)*sin(2.*mu)+(21.*e1*e1/16.-55.*e1*e1*e1*e1/32.)*sin(4.*mu)+(151.*e1*e1*e1/96.)*sin(6.*mu);
//  phi1        = phi1Rad*rad2deg;
    N1          = a/sqrt(1.-eccSquared*sin(phi1Rad)*sin(phi1Rad) );
    T1          = tan(phi1Rad)*tan(phi1Rad);
    C1          = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
    R1          = a*(1.-eccSquared)/pow(1.-eccSquared*sin(phi1Rad)*sin(phi1Rad),1.5);
    D           = dUTMEasting/(N1*k0);

    dLatitude   = rad2deg*(phi1Rad-(N1*tan(phi1Rad)/R1)*(D*D/2.-(5.+3.*T1+10.*C1-4.*C1*C1-9.*eccPrimeSquared)*D*D*D*D/24.+(61.+90.*T1+298.*C1+45.*T1*T1-252.*eccPrimeSquared-3.*C1*C1)*D*D*D*D*D*D/720.));
    dLongitude	= (((double) ZoneNumber-1.)*6.-180.+3.)+rad2deg*((D-(1.+2.*T1+C1)*D*D*D/6.+(5.-2.*C1+28.*T1-3.*C1*C1+8.*eccPrimeSquared+24.*T1*T1)*D*D*D*D*D/120.)/cos(phi1Rad));

    if ( digitsLL > 0 )
    {
        latitude  = QString( "%1" ).arg( dLatitude, 0, 'f', digitsLL );
        longitude = QString( "%1" ).arg( dLongitude, 0, 'f', digitsLL );
    }
    else
    {
        convertLLtoLL( QString( "%1" ).arg( dLatitude, 0, 'f', 7 ), QString( "%1" ).arg( dLongitude, 0, 'f', 7 ) );
    }

    if ( sep == _COMMA_ )
    {
        latitude.replace( ".", "," );
        longitude.replace( ".", "," );
    }

    return( err );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/*Reference ellipsoids derived from Peter H. Dana's website-
http://www.utexas.edu/depts/grg/gcraft/notes/datum/elist.html
Department of Geography, University of Texas at Austin
Internet: pdana@mail.utexas.edu
3/22/95

Source
Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to Department of Defense World Geodetic System
1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency

Converts lat/long to UTM coords. Equations from USGS Bulletin 1532 (hdl:10013/epic.46739.d001)
East Longitudes are positive, West longitudes are negative.
North latitudes are positive, South latitudes are negative
Lat and Long are in decimal degrees
Written by Chuck Gant - mailto:chuck.gantz@glob:alstar.com
rewritten for Qt/C++ by Rainer Sieger - rsieger@pangaea.de
*/

int convertPosition::convertLLtoUTM( const QString &s_Latitude, const QString &s_Longitude, const int i_ReferenceEllipsoid )
{
    QString sd_Latitude  = s_Latitude;
    QString sd_Longitude = s_Longitude;

    sd_Latitude.replace( ",", "." );
    sd_Longitude.replace( ",", "." );

    double  dLatitude  = sd_Latitude.toDouble();
    double  dLongitude = sd_Longitude.toDouble();

    latitude  = QString( "%1" ).arg( dLatitude, 0, 'f', 7 );
    longitude = QString( "%1" ).arg( dLongitude, 0, 'f', 7 );

// **********************************************************************************************

    int	   err              = _NOERROR_;

    double k0				= 0.9996;
    double a				= v_Ellipsoid.at( i_ReferenceEllipsoid ).EquatorialRadius();
    double eccSquared		= v_Ellipsoid.at( i_ReferenceEllipsoid ).EccentricitySquared();
    double eccPrimeSquared	= (eccSquared)/(1.-eccSquared);
    double N, T, C, A, M;

    double PI				= 3.14159265;
    double deg2rad			= PI/180.;

    double LatRad			= 0.;
    double LongRad			= 0.;

    double dUTMNorthing		= 0.;
    double dUTMEasting		= 0.;

    int    ZoneNumber		= 0;

/*****************************************************************************/

    if ( ( dLatitude < -80. ) || ( dLatitude > 84. ) )
        err = -1;

    if ( ( dLongitude < -180. ) || ( dLongitude >= 180. ) )
        err = -2;

    if ( err != _NOERROR_ )
    {
        northing = -999.999;
        easting  = -999.999;
        zone     = "Z";
        return( err );
    }

/*****************************************************************************/

    ZoneNumber = (int)( ( dLongitude + 180. )/6. ) + 1;

    LatRad     = dLatitude*deg2rad;
    LongRad    = dLongitude*deg2rad;

    if ( ( dLatitude >= 56.0 ) && ( dLatitude < 64.0 ) && ( dLongitude >= 3.0 ) && ( dLongitude < 12.0 ) )
        ZoneNumber = 32;

  // Special zones for Svalbard
    if ( ( dLatitude >= 72.0 ) && ( dLatitude < 84.0 ) )
    {
      if ( ( dLongitude >= 0.0 ) && ( dLongitude <  9.0 ) ) ZoneNumber = 31;
      else if ( ( dLongitude >= 9.0 ) && ( dLongitude < 21.0 ) ) ZoneNumber = 33;
      else if ( ( dLongitude >= 21.0 ) && ( dLongitude < 33.0 ) ) ZoneNumber = 35;
      else if ( ( dLongitude >= 33.0 ) && ( dLongitude < 42.0 ) ) ZoneNumber = 37;
    }

    N = a/sqrt(1.-eccSquared*sin(LatRad)*sin(LatRad) );
    T = tan(LatRad)*tan(LatRad);
    C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
    A = cos(LatRad)*(LongRad-deg2rad*(((double) ZoneNumber-1.)*6.-180.+3.));
    M = a*((1.-eccSquared/4.-3.*eccSquared*eccSquared/64.-5.*eccSquared*eccSquared*eccSquared/256.)*LatRad-(3.*eccSquared/8.+3.*eccSquared*eccSquared/32.+45.*eccSquared*eccSquared*eccSquared/1024.)*sin(2.*LatRad)+(15.*eccSquared*eccSquared/256.+45.*eccSquared*eccSquared*eccSquared/1024.)*sin(4.*LatRad)-(35.*eccSquared*eccSquared*eccSquared/3072.)*sin(6.*LatRad));

    dUTMEasting  = k0*N*(A+(1.-T+C)*A*A*A/6.+(5.-18.*T+T*T+72.*C-58.*eccPrimeSquared)*A*A*A*A*A/120.)+500000.;
    dUTMNorthing = k0*(M+N*tan(LatRad)*(A*A/2.+(5.-T+9.*C+4.*C*C)*A*A*A*A/24.+(61.-58.*T+T*T+600.*C-330.*eccPrimeSquared)*A*A*A*A*A*A/720.));

    if ( dLatitude < 0 )
        dUTMNorthing += 10000000.; //10000000 meter offset for southern hemisphere

    northing = dUTMNorthing;
    easting  = dUTMEasting;
    zone  	 = QString( "%1%2" ).arg( ZoneNumber ).arg( UTMLetterDesignator( dLatitude ) );

    return( err );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

int convertPosition::getZone( const QString &UTMZone, int &ZoneNumber, int &ZoneLetter )
{
    if ( ( UTMZone.isEmpty() == true ) || ( UTMZone == "_ERROR_" ) )
        return (-1);

    int l = UTMZone.length();

    ZoneLetter = UTMZone.at( l-1 ).toUpper().toLatin1();

    if ( ( 48 <= ZoneLetter ) && ( ZoneLetter <= 57 ) ) // ZoneLetter between 0,..,9
    {
        ZoneNumber = UTMZone.left( l ).toInt();
        ZoneLetter = 78;
    }
    else
        ZoneNumber = UTMZone.left( l-1 ).toInt();

    if ( ( ZoneLetter == 73 ) || ( ZoneLetter < 67 ) )
        ZoneLetter = 74;

    if ( ( ZoneLetter == 79 ) || ( ZoneLetter > 88 ) )
        ZoneLetter = 78;

    if ( ( ZoneNumber < 1 ) || ( ZoneNumber > 60 ) )
        return (-2);

    return( 0 );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

QString convertPosition::UTMLetterDesignator( const double Lat )
{
//This routine determines the correct UTM letter designator for the given latitude
//returns "Z" if latitude is outside the UTM limits of 84N to 80S
//Written by Chuck Gantz - chuck.gantz@globalstar.com
//rewritten for Qt/C++ by Rainer Sieger - rsieger@pangaea.de


    if ( Lat > 84 ) return( "Z" ) ;
    if ( Lat < -80 ) return( "Z" );

    if ( (  84 >= Lat ) && ( Lat >=  72 ) ) return( "X" );
    if ( (  72 >  Lat ) && ( Lat >=  64 ) ) return( "W" );
    if ( (  64 >  Lat ) && ( Lat >=  56 ) ) return( "V" );
    if ( (  56 >  Lat ) && ( Lat >=  48 ) ) return( "U" );
    if ( (  48 >  Lat ) && ( Lat >=  40 ) ) return( "T" );
    if ( (  40 >  Lat ) && ( Lat >=  32 ) ) return( "S" );
    if ( (  32 >  Lat ) && ( Lat >=  24 ) ) return( "R" );
    if ( (  24 >  Lat ) && ( Lat >=  16 ) ) return( "Q" );
    if ( (  16 >  Lat ) && ( Lat >=   8 ) ) return( "P" );
    if ( (   8 >  Lat ) && ( Lat >=   0 ) ) return( "N" );
    if ( (   0 >  Lat ) && ( Lat >=  -8 ) ) return( "M" );
    if ( (  -8 >  Lat ) && ( Lat >= -16 ) ) return( "L" );
    if ( ( -16 >  Lat ) && ( Lat >= -24 ) ) return( "K" );
    if ( ( -24 >  Lat ) && ( Lat >= -32 ) ) return( "J" );
    if ( ( -32 >  Lat ) && ( Lat >= -40 ) ) return( "H" );
    if ( ( -40 >  Lat ) && ( Lat >= -48 ) ) return( "G" );
    if ( ( -48 >  Lat ) && ( Lat >= -56 ) ) return( "F" );
    if ( ( -56 >  Lat ) && ( Lat >= -64 ) ) return( "E" );
    if ( ( -64 >  Lat ) && ( Lat >= -72 ) ) return( "D" );
    if ( ( -72 >  Lat ) && ( Lat >= -80 ) ) return( "C" );

    return( "Z" ); //This is here as an error flag to show that the Latitude is outside the UTM limits
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void convertPosition::initReferenceEllipsoid( QVector<Ellipsoid> &v_Ellipsoid )
{
    v_Ellipsoid.clear();

    v_Ellipsoid.append( Ellipsoid( "Airy",                   6377563., 0.006670540 ) );   // 0
    v_Ellipsoid.append( Ellipsoid( "Australian National",    6378160., 0.006694542 ) );   // 1
    v_Ellipsoid.append( Ellipsoid( "Bessel 1841",            6377397., 0.006674372 ) );   // 2
    v_Ellipsoid.append( Ellipsoid( "Bessel 1841 (Nambia)",   6377484., 0.006674372 ) );   // 3
    v_Ellipsoid.append( Ellipsoid( "Clarke 1866",            6378206., 0.006768658 ) );   // 4
    v_Ellipsoid.append( Ellipsoid( "Clarke 1880",            6378249., 0.006803511 ) );   // 5
    v_Ellipsoid.append( Ellipsoid( "Everest",                6377276., 0.006637847 ) );   // 6
    v_Ellipsoid.append( Ellipsoid( "Fischer 1960 (Mercury)", 6378166., 0.006693422 ) );   // 7
    v_Ellipsoid.append( Ellipsoid( "Fischer 1968",           6378150., 0.006693422 ) );   // 8
    v_Ellipsoid.append( Ellipsoid( "GRS-67",                 6378160., 0.006694605 ) );   // 9
    v_Ellipsoid.append( Ellipsoid( "GRS-80",                 6378137., 0.00669438002 ) ); // 10  a = 6378137 m, f = 1/298.257222101, e2 = 0.00669438002
    v_Ellipsoid.append( Ellipsoid( "Helmert 1906",           6378200., 0.006693422 ) );   // 11
    v_Ellipsoid.append( Ellipsoid( "Hough",                  6378270., 0.006722670 ) );   // 12
    v_Ellipsoid.append( Ellipsoid( "International",          6378388., 0.006722670 ) );   // 13
    v_Ellipsoid.append( Ellipsoid( "Krassovsky",             6378245., 0.006693422 ) );   // 14
    v_Ellipsoid.append( Ellipsoid( "Modified Airy",          6377340., 0.006670540 ) );   // 15
    v_Ellipsoid.append( Ellipsoid( "Modified Everest",       6377304., 0.006637847 ) );   // 16
    v_Ellipsoid.append( Ellipsoid( "Modified Fischer 1960",  6378155., 0.006693422 ) );   // 17
    v_Ellipsoid.append( Ellipsoid( "South American 1969",    6378160., 0.006694542 ) );   // 18
    v_Ellipsoid.append( Ellipsoid( "WGS-60",                 6378165., 0.006693422 ) );   // 19
    v_Ellipsoid.append( Ellipsoid( "WGS-66",                 6378145., 0.006694542 ) );   // 20
    v_Ellipsoid.append( Ellipsoid( "WGS-72",                 6378135., 0.006694318 ) );   // 21
    v_Ellipsoid.append( Ellipsoid( "WGS-84",                 6378137., 0.00669437999 ) ); // 22  a = 6378137 m, f = 1/298.257223563, e2 = f(2-f) = 0.00669437999
    v_Ellipsoid.append( Ellipsoid( "ETRS-89",                6378137., 0.00669438002 ) ); // 23  a = 6378137 m, f = 1/298.257222101, e2 = f(2-f) = 0.00669438002
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

//-------------------------------------------------------------
//
//  THIS SUBROUTINE WAS MODIFIED FROM SUBROUTINE KOAB
//  DEVELOPED BY F.M. MONKA, INST. OF THEORETICAL GEODESY,
//  HANNOVER, _VERSION_: MAY 1979
//
//  INPUT:    IMODE  = SELECTOR SWITCH
//                   = 1 : UTM
//                   = 2 : GAUSS KRUEGER
//            A  = SEMIMAJOR AXIS
//            B  = SEMIMINOR AXIS
//
//  OUTPUT:   X  = PLANE COORDINATE
//            Y  = PLANE COORDINATE
//            CG = MERIDIAN CONVERGENCE
//
//
//-- THIS PROGRAM TRANSFORMES GEOGRAPHICAL COORDINATES INTO
//-- GAUSS-KRUEGER OR  UTM  COORDINATES
//
//   For C by Dr. Rainer Sieger, AWI
//   1998-01-28
//
//-------------------------------------------------------------

int convertPosition::convertLLtoGK( const QString &s_Latitude, const QString &s_Longitude )
{
    QString sd_Latitude  = s_Latitude;
    QString sd_Longitude = s_Longitude;

    sd_Latitude.replace( ",", "." );
    sd_Longitude.replace( ",", "." );

    double  d_Latitude  = sd_Latitude.toDouble();
    double  d_Longitude = sd_Longitude.toDouble();

    latitude  = QString( "%1" ).arg( d_Latitude, 0, 'f', 7 );
    longitude = QString( "%1" ).arg( d_Longitude, 0, 'f', 7 );

// **********************************************************************************************

    int     IMODE       = 2;

    int		err         = _NOERROR_;

    int		iZFK		= 0;

    double  pi			= 0.;
    double  rho			= 0.;

    double  A			= 0.;
    double  B			= 0.;
    double  AL			= 0.;
    double  AM0			= 0.;
    double  AN			= 0.;
    double  BELT		= 0.;
    double  ETAQ		= 0.;
    double  EXZQ		= 0.;
    double  G			= 0.;
    double  Longitude0	= 0.;
    double  QM			= 0.;
    double  QN			= 0.;
    double  tanLat		= 0.;
    double  sinLat		= 0.;
    double  cosLat		= 0.;
    double  sqrtSAVE	= 0.;

    double  AN2			= 0.;
    double  AN3			= 0.;
    double  AN4			= 0.;
    double  AL2			= 0.;
    double  AL3			= 0.;
    double  AL4			= 0.;
    double  AL5			= 0.;
    double  d1			= 0.;
    double  d2			= 0.;
    double  d3			= 0.;
    double  d4			= 0.;
    double  d5			= 0.;
//  double  d6			= 0.;
    double  cL2			= 0.;
    double  cL3			= 0.;
    double  cL4			= 0.;
    double  cL5			= 0.;
    double  TT			= 0.;

//-------------------------------------------------------------

    if ( d_Longitude < 0 )
        d_Longitude += 360.;

      pi  = 4.*atan(1.);
      rho = 180./pi;

      switch (IMODE)
      {
      case 1:
//......Internationales Ellipsoid, grosse und kleine Halbachse
//
        A     = 6378388.0;
        B     = 6356911.94613;
        AM0   = 0.9996;
        BELT  = 6.;
        break;

      case 2:
//......Gauss-Krueger
//
        A     = 6377397.155;  // Bessel 1841
        B     = 6356078.96325;
        AM0   = 1.;
        BELT  = 3.;
        break;
      }

      d_Latitude = d_Latitude/rho;
      sinLat     = sin( d_Latitude );
      cosLat     = cos( d_Latitude );
      tanLat     = tan( d_Latitude );

      EXZQ 	   = (A*A-B*B)/(A*A);
      sqrtSAVE = sqrt((1. - EXZQ*sinLat*sinLat) );

      AN  = (A-B)/(A+B);
      AN2 = AN*AN;
      AN3 = AN2*AN;
      AN4 = AN2*AN2;

//--COMPUTATION OF CENTRAL MERIDIAN
      switch (IMODE)
      {
      case 1:
        iZFK 	   = (int) ( d_Longitude/BELT + 31. );
        Longitude0 = (double) iZFK * BELT - BELT/2. - 180.;
        break;
      default:
        iZFK 	   = (int) ( ( d_Longitude + BELT/2. )/BELT );
        Longitude0 = (double) iZFK * BELT;
        break;
      }

//--LENGTH OF MERIDIAN
      G = A/(1. + AN) * ((1. + AN2/4. + AN4/64.)*d_Latitude
              - (AN - AN3/8.)*1.5*sin(2.*d_Latitude)
              + (AN2 - AN4/4.)*15./16.*sin(4.*d_Latitude)
              - AN3*35./48.*sin(6.*d_Latitude)
              + AN4*315./512.*sin(8.*d_Latitude) );

      AL  = ( d_Longitude - Longitude0 )/rho;
      AL2 = AL*AL;
      AL3 = AL2*AL;
      AL4 = AL2*AL2;
      AL5 = AL3*AL2;

      QN   = A/sqrtSAVE;
      QM   = A*(1. - EXZQ)/(sqrtSAVE*sqrtSAVE*sqrtSAVE);
      ETAQ = QN/QM - 1.;
      TT   = tanLat*tanLat;

//--COMPUTE COEFFICIENTS
      cL2 = cosLat*cosLat;
      cL3 = cL2*cosLat;
      cL4 = cL2*cL2;
      cL5 = cL3*cL2;
      d1  = QN*cosLat;
      d2  = QN*cL2*tanLat/2.;
      d3  = QN*cL3*(1. - TT + ETAQ)/6.;
      d4  = QN*cL4*tanLat*(5. - TT + 9.*ETAQ)/24.;
      d5  = QN*cL5*(5. - 18.*TT + TT*TT)/120.;
//    d6  = sinLat*(cL2)*(1. + 3.*ETAQ)/3.;

//--COMPUTE  CG
//   *CG = sinLat*AL + d6*AL3;

//--COMPUTE COORDINATES
      hoch     = (d2*AL2 + d4*AL4 + G) * AM0;
      rechts   = (d1*AL + d3*AL3 + d5*AL5)*AM0 + 500000.;// + (double) iZFK*1000000.;
      streifen = iZFK;

      return( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

// *****************************************************************************
//     Aus dem Programm GL selektiert und modifiziert von                      *
//                                                                             *
//            Fred Niederjasper                                                *
//            Alfred-Wegener-Institut fuer Polar- und Meeresforschung          *
//            D - 2850 Bremerhaven                                             *
//                                                                             *
//            Fuer C: Dr. Rainer Sieger                                        *
//                                                                             *
//     Stand: 1990-12-20                                                       *
// *****************************************************************************
//
//    SUBROUTINE GAUTR (IMODE,A,DE,DH,DR,DB,DL)
//
//    MODIFIED FROM PROGRAM MG1, H.G.WENZEL,
//                               INSTITUTE OF THEORETI//AL GEODESY
//                               UNIVERSITY HANNOVER
//
//    MODIFICATIONS
//    1)  DOUBLE PRECISION
//    2)  TRANSFER OF A AND DE FROM CALLING ROUTINE
//    3)  INTRODUCTION OF IMODE
//    4)  USE OF GEODETIC DIRECT SOLUTION FOR SOLVING
//        _LATITUDE BELONGING TO NORTH VALUE
//
//
//    INPUT:  IMODE = SELECTOR SWITCH
//                  = 1 : UTM
//                  = 2 : GAUSS KRUEGER
//            A  =  SEMIMAJOR AXIS (METERS)
//            DE =  ESQUARED
//            DR =  EAST VALUE
//            DH =  NORTH VALUE
//
//    OUTPUT: DB =  _LATITUDE
//            DL =  _LONGITUDE
//
//    THIS SUBROUTINE CALLS SUBROUTINE DIVIN
//
//---------------------------------------------------------------

int convertPosition::convertGKtoLL( const QString &s_Streifen, const QString &s_Rechts, const QString &s_Hoch )
{
    QString sd_Rechts = s_Rechts;
    QString sd_Hoch   = s_Hoch;

    sd_Rechts.replace( ",", "." );
    sd_Hoch.replace( ",", "." );

    double  d_Rechts = sd_Rechts.toDouble();
    double  d_Hoch   = sd_Hoch.toDouble();

    streifen        = s_Streifen.toInt();
    rechts          = d_Rechts;
    hoch            = d_Hoch;

// **********************************************************************************************

    int		err     = _NOERROR_;
    int     IMODE   = 2;

    double dR       = 1000000.*(double) s_Streifen.toInt() + d_Rechts;
    double dH       = d_Hoch;

    double  AJI		= 0.;
    double  AM0		= 0.;
    double  B		= 0.;
    double  A		= 0.;
    double  DB		= 0.;
    double  DC1		= 0.;
    double  DC2		= 0.;
    double  DC3		= 0.;
    double  DC4		= 0.;
    double  DC5		= 0.;
    double  DCB		= 0.;
    double  DE		= 0.;
    double  DL		= 0.;
    double  DM		= 0.;
    double  DN		= 0.;
    double  DNN		= 0.;
    double  rad		= 0.;
    double  DSB		= 0.;
    double  DTB		= 0.;
    double  DY		= 0.;
    double  pi		= 0.;
    double  rho		= 0.;
    double  SIJ		= 0.;
    double  XLONJ	= 0.;

    int     IL		= 0;
    int     ILL		= 0;

    double	DTB2	= 0.;
    double  DY2		= 0.;
    double  DY3		= 0.;
    double  DN2		= 0.;
    double  DN3		= 0.;

//-------------------------------------------------------------

      pi   = 4.*atan(1.);
      rho  = 180./pi;
      rad  = 1./rho;

      switch (IMODE)
      {
      case 1:
//......Internationales Ellipsoid, grosse und kleine Halbachse
//
          A    = 6378388.0;
          B    = 6356911.94613;
          AM0  = 0.9996;
          IL   = (int) (dR*1.E-6);
          ILL  = 6*IL - 180 - 3;
          DL   = (double) ILL;
          DY   = dR - (double) IL * 1.E6 - 0.5E6;
          DY   = DY/AM0;
          break;

      case 2:
//......Gauss-Krueger
          A    = 6377397.155; // Bessel 1841
          B    = 6356078.96325;
          AM0  = 1.;
          IL   = (int) (dR*1.E-6);
          DL   = 3.* (double) IL;
          DY   = dR - (double) IL * 1.E6 - 0.5E6;
          break;
      }

      DY2    = DY*DY;
      DY3    = DY2*DY;
      DE     = 1. - ((B*B)/(A*A) );

      SIJ    = fabs(dH/AM0);

      DIVIN(A,B,0.,0.7,0.,SIJ,&DB,&XLONJ,&AJI);

      DSB    = sin(DB);
      DCB    = cos(DB);
      DB     = DB/rad;
      DNN    = 1. - DE*DSB*DSB;
      DM     = A*(1. - DE)/sqrt(DNN*DNN*DNN);
      DN     = A/sqrt(DNN);
      DN2    = DN*DN;
      DN3    = DN2*DN;
      DTB    = DSB/DCB;
      DTB2   = DTB*DTB;
      DC1    = 1./(DN*DCB*rad);
      DC2    = -DTB/(2.*DN*DM*rad);
      DC3    = -1.*(2.*DTB2+DN/DM)/(6.*DN3*DCB*rad);
      DC4    = DTB*(-1. + 9.*DTB2 + 6.*DN/DM*(1. - DTB2) )/(24.*DN2*DN2*rad);
      DC5    = (5. + 14.*DTB2 + 9.*DTB2*DTB2)/(120.*DN2*DN3*rad);
      DL     = DL + DY*DC1 + DY3*DC3 + DY2*DY3*DC5;
      DB     = DB + DY2*DC2 + DY3*DY*DC4;

      if (dH < 0.)
        DB = -DB;
      if (DL > 180.)
        DL -= 360.;

      if ( digitsLL > 0 )
      {
          latitude  = QString( "%1" ).arg( DB, 0, 'f', digitsLL );
          longitude = QString( "%1" ).arg( DL, 0, 'f', digitsLL );
      }
      else
      {
          convertLLtoLL( QString( "%1" ).arg( DB, 0, 'f', 7 ), QString( "%1" ).arg( DL, 0, 'f', 7 ) );
      }

      if ( sep == _COMMA_ )
      {
          latitude.replace( ".", "," );
          longitude.replace( ".", "," );
      }

      return( err );
}

/***********************************************************************
*                                                                     *
*       SUBROUTINE DIVIN  SOLVES THE DIRECT PROBLEM IN GEODESY, THAT  *
*  IS, GIVEN THE _LATITUDE AND LONGTITUDE OF A POINT I, PLUS THE       *
*  AZIMUTH AND _DISTANCE TO A POINT J, COMPUTE THE _LATITUDE AND        *
*  LONGTITUDE OF J, PLUS THE BACKWARD AZIMUTH FROM J TO I.            *
*       THE SOLUTION IS ITERATED UNTIL THE CHANGE IN SIGMA IS LESS    *
*  THAN 1.D-12                                                        *
*  NOTE  THE EQUIVALENT INVERSE PROGRAM IS SUBROUTINE INVIN           *
*                                                                     *
*  REFERENCE  T. VINCENTY - SURVEY REVIEW, APRIL 1975                 *
*                                                                     *
*   INPUT DATA                                                        *
*              A=SEMI-MAJOR AXIS OF REFERENCE ELLIPSOID IN METRES     *
*              B=SEMI-MINOR AXIS OF REFERENCE ELLIPSOID OR THE        *
*                RECIPROCAL OF THE FLATTENING IN METRES OR UNITLESS   *
*              PHII=GEODETIC _LATITUDE OF POINT I IN RADIANS           *
*              XLONI=GEODETIC LONGTITUDE OF POINT I IN RADIANS        *
*              AIJ=GEODETIC AZIMUTH FROM I TO J IN RADIANS            *
*                NOTE  AZIMUTH ON ELLIPSOID                           *
*              SIJ=_DISTANCE FROM I TO J IN METRES                     *
*                NOTE  _DISTANCE ON ELLIPSOID                          *
*                                                                     *
*  OUTPUT DATA                                                        *
*              PHIJ=GEODETIC _LATITUDE OF POINT J IN RADIANS           *
*              XLONJ=GEODETIC LONGTITUDE OF POINT J IN RADIANS        *
*              AJI=GEODETIC AZIMUTH FROM J TO I IN RADIANS            *
*                NOTE  AZIMUTH ON ELLIPSOID                           *
*                                                                     *
*                                     INGE NESBO  JAN. 1976           *
*                                                                     *
**********************************************************************/

void convertPosition::DIVIN( double A, double B, double PHII, double XLONI, double AIJ, double SIJ, double *PHIJ, double *XLONJ, double *AJI )
{
//
//  WORK IN DOUBLE PRECISION
//
//  IMPLICIT DOUBLE PRECISION (A-H),DOUBLE PRECISION (O-Z)
//
//  CHECK IF ELLIPSOID DEFINED BY A + B OR A + 1/F AND COMPUTE
//  ECCENTRICITY
//

    double  AC		= 0.;
    double  B1		= 0.;
    double  BC		= 0.;
    double  BONA	= 0.;
    double  C		= 0.;
    double  CA		= 0.;
    double  CDL		= 0.;
    double  CDSIG	= 0.;
    double  CP		= 0.;
    double  CSIG1	= 0.;
    double  CSIG	= 0.;
    double  CSQALF	= 0.;
    double  CU1		= 0.;
    double  DDSIG	= 0.;
    double  DEL		= 0.;
    double  DDL		= 0.;
    double  DIAG	= 0.;
    double  DSIG	= 0.;
    double  DSIGM	= 0.;
//  double  ESQ1	= 0.;
    double  ESQ2	= 0.;
    double  F		= 0.;
    double  SA		= 0.;
    double  SALF	= 0.;
    double  SDL		= 0.;
//  double  SDSIG	= 0.;
    double  SIG0	= 0.;
    double  SIG1	= 0.;
    double  SIG		= 0.;
    double  SP		= 0.;
    double  SSIG1	= 0.;
    double  SSIG	= 0.;
    double  SU1		= 0.;
    double  USQ		= 0.;

    int     IK		= 0;

//-------------------------------------------------------------

      B1 = B;
      if (B1 < 1000.)
        B1 = A-A/B1;

      F    = (A-B1)/A;
//    ESQ1 = (A*A-B1*B1)/A/A;
      ESQ2 = (A*A-B1*B1)/B1/B1;
      BONA = B1/A;
//
      SA    = sin(AIJ);
      CA    = cos(AIJ);
      SP    = sin(PHII)*BONA;
      CP    = cos(PHII);
      DIAG  = sqrt(SP*SP+CP*CP);
      SU1   = SP/DIAG;
      CU1   = CP/DIAG;
      SSIG1 = SU1;
      CSIG1 = CU1*CA;
      SIG1  = atan2(SSIG1,CSIG1);
      SALF  = CU1*SA;
//
      USQ   = (1. - SALF*SALF)*ESQ2;
      AC    = -768. + USQ*(320. - 175.*USQ);
      AC    = 1. + USQ*(4096. + USQ*AC)/16384.;
      BC    = -128. + USQ*(74. - 47.*USQ);
      BC    = USQ*(256. + USQ*BC)/1024.;
//
//     TESTVALUE FOR STOP OF ITERATION
      DEL   = 1.E-12;
//
      DDSIG = 0.;
      SIG0  = SIJ/B1/AC;
      SIG   = SIG0;
      IK    = -1;
//
//     START OF ITERATION
//
      while (IK <= 0)
      {
          DSIG  = DDSIG;
          DSIGM = 2.*SIG1 + SIG;
          SSIG  = sin(SIG);
          CSIG  = cos(SIG);
//        SDSIG = sin(DSIGM);
          CDSIG = cos(DSIGM);
          if (IK <= 0)
          {
            DDSIG = BC*CDSIG*(-3. + 4.*SSIG*SSIG)*(-3. + 4.*CDSIG*CDSIG)/6.;
            DDSIG = BC*SSIG*(CDSIG + BC*(CSIG*(-1. + 2.*CDSIG*CDSIG)-DDSIG)/4.);
            SIG   = SIG0+DDSIG;
            if (fabs(DDSIG-DSIG) <= DEL)
                IK = 1;
          }
      }

//
//  COMPUTE _LATITUDE OF POINT J
//
      SP    = SU1*CSIG + CU1*SSIG*CA;
      CP    = SU1*SSIG - CU1*CSIG*CA;
      CP    = (1. - F)*sqrt(SALF*SALF + CP*CP);
      *PHIJ = atan2(SP,CP);
//
//    COMPUTE _LONGITUDE AT PIONT J
//
      SDL    = SSIG*SA;
      CDL    = CU1*CSIG - SU1*SSIG*CA;
      CSQALF = 1. - SALF*SALF;
      C      = CSQALF*(4. + F*(4. - 3.*CSQALF) )*F/16.;
      DDL    = CDSIG+C*CSIG*(-1. + 2.*CDSIG*CDSIG);
      DDL    = (1. - C)*F*SALF*(SIG + C*SSIG*DDL);
      *XLONJ = XLONI + atan2(SDL,CDL) - DDL;
//
//     COMPUTE AZIMUT AT POINT J
//
      SA  = SALF;
      CA  = -SU1*SSIG + CU1*CSIG*CA;
      *AJI = atan2(SA,CA);
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int convertPosition::convertLLtoLL( const QString &s_Latitude, const QString &s_Longitude )
{
    latitude                = s_Latitude.simplified();
    longitude               = s_Longitude.simplified();

    int		err             = _NOERROR_;

    int		Direction       = 1;
    int     iPos            = 0;

    double	dPos            = 0.;
    double	gPos            = 0;
    double  mPos            = 0.;
    double  sPos            = 0;
    double  mf              = 60.;

    double	Coordinate      = 0.0;

/*****************************************************************************/
// Latitude

    if ( ( err == _NOERROR_ ) && ( s_Latitude.isEmpty() == false ) )
    {
        switch ( getMode( latitude ) )
        {
        case 0:
            Direction = 1;
            dPos      = latitude.toDouble();

            if ( dPos < 0 )
            {
                Direction	= -1;
                dPos		= -dPos;
            }

            iPos  = (int) dPos;
            mPos  = ( dPos - (double) iPos )*mf;
            sPos  = ( mPos - (int) mPos )*mf;

            if ( ( mPos < 0 ) || ( mPos >= 60 ) )
                err = -1;

            if ( ( sPos < 0 ) || ( sPos >= 60 ) )
                err = -2;

            if ( ( iPos < -90 ) || ( iPos > 90 ) )
                err = -3;

            if ( err == _NOERROR_ )
            {
                if ( digitsLL > 0 )
                {
                    latitude = QString( "%1°%2'" ).arg( iPos ).arg( mPos, 2, 'f', digitsLL );
                }
                else
                {
                    latitude = QString( "%1°%2'%3''" ).arg( iPos ).arg( (int) mPos, 2).arg( sPos, 2, 'f', 2 );
                    latitude.replace( " ", "0" );
                }

                if ( Direction > 0 )
                    latitude.append( " N" );
                else
                    latitude.append( " S" );
            }
            break;

        case 1:
            if ( ( latitude.startsWith( "\"" ) == true ) && ( latitude.endsWith( "\"" ) == true ) )
                latitude = latitude.mid( 1, latitude.length() - 2 );

            Direction = 1;

            if ( latitude.contains( "s", Qt::CaseInsensitive ) == true )
                Direction = -1;

            if ( latitude.contains( "-" ) == true )
                Direction = -1;

            latitude.replace( ",", "." );
            latitude.replace( "°", "," );
            latitude.replace( "º", "," );
            latitude.replace( "°", "," );
            latitude.replace( "deg", "," );
            latitude.replace( "d", "," );
            latitude.replace( "´´", "," );
            latitude.replace( "``", "," );
            latitude.replace( "''", "," );
            latitude.replace( "\"", "," );
            latitude.replace( "´", "," );
            latitude.replace( "`", "," );
            latitude.replace( "'", "," );
            latitude.replace( "-", "" );
            latitude.replace( "+", "" );
            latitude.replace( " ", "" );

            latitude.replace( "n", "0" );
            latitude.replace( "N", "0" );
            latitude.replace( "s", "0" );
            latitude.replace( "S", "0" );

            gPos = latitude.section( ",", 0, 0 ).toDouble();
            mPos = latitude.section( ",", 1, 1 ).toDouble();
            sPos = latitude.section( ",", 2, 2 ).toDouble();

            if ( ( mPos >= 100 ) && ( mPos < 1000 ) )
                mPos /= 10.;

            if ( mPos >= 1000 )
                mPos /= 100.;

            mPos += sPos/60.;
            mPos /= 60.;

            Coordinate = gPos + mPos;
            Coordinate *= (double) Direction;

            if ( ( Coordinate < -90.0 ) || ( Coordinate > 90.0 ) )
                err = -3;

            if ( err == _NOERROR_ )
                latitude = QString( "%1" ).arg( Coordinate , 0, 'f', digitsLL );
            else
                latitude = "";
            break;

        default:
            latitude = "";
            break;
        }
    }

/*****************************************************************************/
// Longitude

    if ( ( err == _NOERROR_ ) && ( s_Longitude.isEmpty() == false ) )
    {
        switch ( getMode( longitude ) )
        {
        case 0:
            Direction = 1;
            dPos      = longitude.toDouble();

            if ( dPos > 180. )
                dPos -= 360.;

            if ( dPos < 0. )
            {
                Direction	= -1;
                dPos		= -dPos;
            }

            iPos  = (int) dPos;
            mPos  = ( dPos - (double) iPos )*mf;
            sPos  = ( mPos - (int) mPos )*mf;

            if ( ( mPos < 0 ) || ( mPos >= 60 ) )
                err = -1;

            if ( ( sPos < 0 ) || ( sPos >= 60 ) )
                err = -2;

            if ( ( iPos < -180 ) || ( iPos > 180 ) )
                err = -3;

            if ( err == _NOERROR_ )
            {
                if ( digitsLL > 0 )
                {
                    longitude = QString( "%1°%2'" ).arg( iPos ).arg( mPos, 2, 'f', digitsLL );
                }
                else
                {
                    longitude = QString( "%1°%2'%3''" ).arg( iPos ).arg( (int) mPos, 2).arg( sPos, 2, 'f', 2 );
                    longitude.replace( " ", "0" );
                }

                if ( Direction > 0 )
                    longitude.append( " E" );
                else
                    longitude.append( " W" );
            }
            break;

        case 1:
            if ( ( longitude.startsWith( "\"" ) == true ) && ( longitude.endsWith( "\"" ) == true ) )
                longitude = longitude.mid( 1, longitude.length() - 2 );

            Direction = 1;

            if ( longitude.contains( "w", Qt::CaseInsensitive ) == true )
                Direction = -1;

            if ( longitude.contains( "-" ) == true )
                Direction = -1;

            longitude.replace( ",", "." );
            longitude.replace( "°", "," );
            longitude.replace( "º", "," );
            longitude.replace( "°", "," );
            longitude.replace( "deg", "," );
            longitude.replace( "d", "," );
            longitude.replace( "´´", "," );
            longitude.replace( "``", "," );
            longitude.replace( "''", "," );
            longitude.replace( "\"", "," );
            longitude.replace( "´", "," );
            longitude.replace( "`", "," );
            longitude.replace( "'", "," );
            longitude.replace( "-", "" );
            longitude.replace( "+", "" );
            longitude.replace( " ", "" );

            longitude.replace( "e", "0" );
            longitude.replace( "E", "0" );
            longitude.replace( "w", "0" );
            longitude.replace( "W", "0" );

            gPos = longitude.section( ",", 0, 0 ).toDouble();
            mPos = longitude.section( ",", 1, 1 ).toDouble();
            sPos = longitude.section( ",", 2, 2 ).toDouble();

            if ( ( mPos >= 100 ) && ( mPos < 1000 ) )
                mPos /= 10.;

            if ( mPos >= 1000 )
                mPos /= 100.;

            mPos += sPos/60.;
            mPos /= 60.;

            Coordinate = gPos + mPos;
            Coordinate *= (double) Direction;

            if ( ( Coordinate < -180.0 ) || ( Coordinate > 180.0 ) )
                err = -3;

            if ( err == _NOERROR_ )
                longitude = QString( "%1" ).arg( Coordinate , 0, 'f', digitsLL );
            break;

        default:
            longitude = "";
            break;
        }
    }

    if ( ( err == _NOERROR_ ) && ( sep == _COMMA_ ) )
    {
        latitude.replace( ".", "," );
        longitude.replace( ".", "," );
    }

    return( err );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

int convertPosition::getMode( const QString &s_Position )
{
    if ( s_Position.isEmpty() == true )
        return(-1);
    if ( s_Position.contains( "°" ) == true )
        return(1);
    if ( s_Position.contains( "º" ) == true )
        return(1);
    if ( s_Position.contains( "'" ) == true )
        return(1);
    if ( s_Position.contains( "`" ) == true )
        return(1);
    if ( s_Position.contains( "´" ) == true )
        return(1);
    if ( s_Position.contains( "?" ) == true )
        return(1);
    if ( s_Position.contains( "\"" ) == true )
        return(1);
    if ( s_Position.contains( "n", Qt::CaseInsensitive ) == true )
        return(1);
    if ( s_Position.contains( "s", Qt::CaseInsensitive ) == true )
        return(1);
    if ( s_Position.contains( "w", Qt::CaseInsensitive ) == true )
        return(1);
    if ( s_Position.contains( "e", Qt::CaseInsensitive ) == true )
        return(1);
    if ( s_Position.contains( " " ) == true )
        return(1);

    return (0);
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2012-08-17

Ellipsoid::Ellipsoid( const QString &ellipsoidName, const double equatorialRadius, double eccentricitySquared )
{
    setEllipsoidName( ellipsoidName );
    setEquatorialRadius( equatorialRadius );
    setEccentricitySquared( eccentricitySquared );
}
