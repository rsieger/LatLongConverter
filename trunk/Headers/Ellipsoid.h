#ifndef ELLIPSOID_H
#define ELLIPSOID_H

class Ellipsoid
{
public:
    Ellipsoid( const QString &ellipsoidName = "", const double equatorialRadius = 0, double eccentricitySquared = 0 );

    QString EllipsoidName() const { return( ellipsoidName ); }
    double EquatorialRadius() const { return( equatorialRadius ); }
    double EccentricitySquared() const { return( eccentricitySquared ); }

    void setEllipsoidName( const QString &Text = "" ) { ellipsoidName = Text; }
    void setEquatorialRadius( double Value = 0 ) { equatorialRadius = Value; }
    void setEccentricitySquared( double Value = 0 ) { eccentricitySquared = Value; }

private:
    QString ellipsoidName;
    double  equatorialRadius;
    double  eccentricitySquared;
};


#endif // ELLIPSOID_H
