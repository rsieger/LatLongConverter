#ifndef CONVERTPOSITION_H
#define CONVERTPOSITION_H

#include "Ellipsoid.h"

class convertPosition
{
public:
    convertPosition( const int NumOfDigits = 5, const int DecimalSeparator = 4 );

    int convertLLtoLL( const QString &Latitude = "", const QString &Longitude = "" );
    int convertLLtoGK( const QString &Latitude = "", const QString &Longitude = "" );
    int convertLLtoUTM( const QString &Latitude = "", const QString &Longitude = "", const int ReferenceEllipsoid = 22 ); // WGS-84
    int convertUTMtoLL( const QString &Zone = "", const QString &Easting = "", const QString &Northing = "", const int ReferenceEllipsoid = 22 ); // WGS-84
    int convertGKtoLL( const QString &Streifen = "0""", const QString &Rechts = "", const QString &Hoch = "" );

    int getMode( const QString &Position );
    int getZone( const QString &UTMZone, int &ZoneNumber, int &ZoneLetter );

    void DIVIN(double A, double B, double PHII, double XLONI, double AIJ, double SIJ, double *PHIJ, double *XLONJ, double *AJI);
    void initReferenceEllipsoid( QVector<Ellipsoid> &v_Ellipsoid );

    QString UTMLetterDesignator( const double Latitude );

    QString Latitude() const { return( latitude ); }
    QString Longitude() const { return( longitude ); }
    QString Zone() const {return( zone ); }

    int     Streifen() const {return( streifen ); }

    int     NumOfDigitsLL() const { return( digitsLL ); }
    int     NumOfDigitsGK() const { return( digitsGK ); }
    int     NumOfDigitsUTM() const { return( digitsUTM ); }
    int     DecimalSeparator() const { return( sep ); }

    double  Rechts() const { return( rechts ); }
    double  Hoch() const { return( hoch ); }
    double  Easting() const { return( easting ); }
    double  Northing() const { return( northing ); }

    void setLatitude( const QString &Latitude = "" ) { latitude = Latitude; }
    void setLongitude( const QString &Longitude = "" ) { longitude = Longitude; }

    void setStreifen( const int Streifen ) { streifen = Streifen; }
    void setRechts( const double Rechts ) { rechts = Rechts; }
    void setHoch( const double Hoch ) { hoch = Hoch; }

    void setZone( const QString Zone ) { zone = Zone; }
    void setEasting( const double Easting ) { easting = Easting; }
    void setNorthing( const double Northing ) { northing = Northing; }

    void setNumOfDigitsLL( int NumOfDigits = 5 ) { digitsLL = NumOfDigits; }
    void setNumOfDigitsGK( int NumOfDigits = 2 ) { digitsGK = NumOfDigits; }
    void setNumOfDigitsUTM( int NumOfDigits = 2 ) { digitsUTM = NumOfDigits; }
    void setDecimalSeparator( int DecimalSpearator = 4 ) { sep = DecimalSpearator; }

private:
    QString latitude;
    QString longitude;
    QString zone;

    double  rechts;
    double  hoch;
    double  easting;
    double  northing;

    int     streifen;

    int     digitsLL;
    int     digitsGK;
    int     digitsUTM;

    int     sep;

    QVector<Ellipsoid> v_Ellipsoid;
};

#endif // CONVERTPOSITION_H
