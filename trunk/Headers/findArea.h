#ifndef FINDAREA_H
#define FINDAREA_H

class PositionItem
{
public:
    PositionItem( const float Latitude = 0., const float Longitude = 0. );

    double Latitude() const { return( latitude ); }
    double Longitude() const { return( longitude ); }

    void setLatitude( double Latitude = -999. ) { latitude = Latitude; }
    void setLongitude( double Longitude = -999. ) { longitude = Longitude; }

private:
    double latitude;       // Latitude
    double longitude;      // Longitude

};

class AreaItem
{
public:
    AreaItem( const QString AreaName = "", const int StartPosition = 0, const int NumOfPoints = 0, const float minLatitude = 0., const float maxLatitude = 0., const float minLongitude = 0., const float maxLongitude = 0. );

    QString AreaName() const { return( name ); }
    int     StartPosition() const { return( startpos ); }
    int     NumOfPoints() const { return( points ); }
    double  minLatitude() const { return( minLat ); }
    double  maxLatitude() const { return( maxLat ); }
    double  minLongitude() const { return( minLong ); }
    double  maxLongitude() const { return( maxLong ); }

    void setAreaName( QString AreaName = "" ) { name = AreaName; }
    void setStartPosition( int StartPosition = 0 ) { startpos = StartPosition; }
    void setNumOfPoints( int NumOfPoints = 0 ) { points = NumOfPoints; }
    void setMinLatitude( double Latitude = -999. ) { minLat = Latitude; }
    void setMaxLatitude( double Latitude = -999. ) { maxLat = Latitude; }
    void setMinLongitude( double Longitude = -999. ) { minLong = Longitude; }
    void setMaxLongitude( double Longitude = -999. ) { maxLong = Longitude; }

private:
    QString	name;           // Name of area
    int     startpos;       // Position of first item
    int     points;         // Number of points in closed polygon
    double  minLat;         // Minimum latitude
    double  maxLat;         // Maximum latitude
    double  minLong;        // Minimum longitude
    double  maxLong;        // Maximum longitude

};

#endif // FINDAREA_H
