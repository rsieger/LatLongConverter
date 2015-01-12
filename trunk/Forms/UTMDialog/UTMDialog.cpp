// ***********************************************************************************************
// *                                                                                             *
// * UTMDialog.cpp - UTM converter dialog                                                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-08-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "convertPosition.h"
#include "UTMDialog.h"

UTMDialog::UTMDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ShowMap_pushButton, SIGNAL(clicked()), this, SLOT(showMap()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LLtoUTM_pushButton, SIGNAL(clicked()), this, SLOT(convertLLtoUTM()));
    connect( UTMtoLL_pushButton, SIGNAL(clicked()), this, SLOT(convertUTMtoLL()));

    connect( Latitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( Longitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumOfDigitsLL_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( enableOKButton() ) );
    connect( NumOfDigitsUTM_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( enableOKButton() ) );
    connect( Dot_radioButton, SIGNAL( toggled( bool ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void UTMDialog::clearAll()
{
    Latitude_lineEdit->setText( "" );
    Longitude_lineEdit->setText( "" );

    UTM_Northing_lineEdit->setText( "" );
    UTM_Easting_lineEdit->setText( "" );
    UTM_Zone_lineEdit->setText( "" );

    ReferenceEllipsoid_comboBox->setCurrentIndex( 22 );

    NumOfDigitsLL_spinBox->setValue( 5 );
    NumOfDigitsLL_spinBox->setRange( 0, 10 );

    NumOfDigitsUTM_spinBox->setValue( 1 );
    NumOfDigitsUTM_spinBox->setRange( 0, 2 );

    Dot_radioButton->setChecked( true );

    ZoomLevel_spinBox->setValue( 5 );
    ZoomLevel_spinBox->setRange( 1, 20 );

    MapType_1_radioButton->setChecked( true );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void UTMDialog::enableOKButton()
{
    if ( ( Latitude_lineEdit->text().isEmpty() == false ) && ( Longitude_lineEdit->text().isEmpty() == false ) )
        ShowMap_pushButton->setEnabled( true );
    else
        ShowMap_pushButton->setEnabled( false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void UTMDialog::convertLLtoUTM()
{
    int    err = _NOERROR_;

    convertPosition *llutm = new convertPosition();

    if ( Dot_radioButton->isChecked() == true )
        llutm->setDecimalSeparator( _DOT_ );
    else
        llutm->setDecimalSeparator( _COMMA_ );

    if ( ( llutm->getMode( Latitude_lineEdit->text() ) == 1 ) || ( llutm->getMode( Longitude_lineEdit->text() ) == 1 ) )
    {
        llutm->setNumOfDigitsLL( 7 );
        llutm->convertLLtoLL( Latitude_lineEdit->text(), Longitude_lineEdit->text() );

        err = llutm->convertLLtoUTM( llutm->Latitude(), llutm->Longitude(), ReferenceEllipsoid_comboBox->currentIndex() );
    }
    else
    {
        err = llutm->convertLLtoUTM( Latitude_lineEdit->text(), Longitude_lineEdit->text(), ReferenceEllipsoid_comboBox->currentIndex() );
    }

    if ( err == -1 )
    {
        QMessageBox::information( this, tr( "Error" ), tr( "Please note that the latitude is outside of\nthe UTM limits. Please choose a latitude\nbetween 84° N to 80° S." ) );
        return;
    }

    if ( err == -2 )
    {
        QMessageBox::information( this, tr( "Error" ), tr( "Please note that the longitude is outside of\nthe UTM limits. Please choose a longitude\nbetween 180° W to 180° E." ) );
        return;
    }

    if ( llutm->DecimalSeparator() == _COMMA_ )
    {
        UTM_Zone_lineEdit->setText( QString( "%1" ).arg( llutm->Zone() ).replace( ".", "," ) );
        UTM_Easting_lineEdit->setText( QString( "%1" ).arg( llutm->Easting(), 0, 'f', NumOfDigitsUTM_spinBox->value() ).replace( ".", "," ) );
        UTM_Northing_lineEdit->setText( QString( "%1" ).arg( llutm->Northing(), 0, 'f', NumOfDigitsUTM_spinBox->value() ).replace( ".", "," ) );
    }
    else
    {
        UTM_Zone_lineEdit->setText( QString( "%1" ).arg( llutm->Zone() ) );
        UTM_Easting_lineEdit->setText( QString( "%1" ).arg( llutm->Easting(), 0, 'f', NumOfDigitsUTM_spinBox->value() ) );
        UTM_Northing_lineEdit->setText( QString( "%1" ).arg( llutm->Northing(), 0, 'f', NumOfDigitsUTM_spinBox->value() ) );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void UTMDialog::convertUTMtoLL()
{
    convertPosition *utmll = new convertPosition();

    utmll->setNumOfDigitsLL( NumOfDigitsLL_spinBox->value() );

    if ( Dot_radioButton->isChecked() == true )
        utmll->setDecimalSeparator( _DOT_ );
    else
        utmll->setDecimalSeparator( _COMMA_ );

    int err = utmll->convertUTMtoLL( UTM_Zone_lineEdit->text(), UTM_Easting_lineEdit->text(), UTM_Northing_lineEdit->text(), ReferenceEllipsoid_comboBox->currentIndex() );

    if ( err == -1 )
    {
        QMessageBox::information( this, "Error", "Wrong UTM zone." );
        return;
    }

    if ( err == -2 )
    {
        QMessageBox::information( this, "Error", "Please note that the UTM Northing is outside of\nthe UTM limits. Please choose a UTM Northing\nbetween 1118385 to 9999999." );
        return;
    }

    if ( err == -3 )
    {
        QMessageBox::information( this, "Error", "Please note that the UTM Northing is outside of\nthe UTM limits. Please choose a UTM Northing\nbetween 0 to 9328112." );
        return;
    }

    if ( err == -4 )
    {
        QMessageBox::information( this, "Error", "Please note that the UTM Easting is outside of\nthe UTM limits. Please choose a UTM Easting\nbetween 161622 to 804300." );
        return;
    }

    Latitude_lineEdit->setText( utmll->Latitude() );
    Longitude_lineEdit->setText( utmll->Longitude() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void UTMDialog::showMap()
{
    int     i_ZoomLevel   = 0;
    int     i_MapType     = 0;

    int     i_Height      = 0;
    int     i_Width       = 0;

    QString s_Latitude    = "";
    QString s_Longitude   = "";

// ***********************************************************************************************************************

    convertPosition *utmll = new convertPosition();

    utmll->setNumOfDigitsLL( NumOfDigitsLL_spinBox->value() );
    utmll->setDecimalSeparator( _DOT_ );

    if ( utmll->NumOfDigitsLL() == 0 )
    {
        utmll->setNumOfDigitsLL( 1 );
        utmll->convertLLtoLL( Latitude_lineEdit->text(), Longitude_lineEdit->text() );

        s_Latitude  = utmll->Latitude();
        s_Longitude = utmll->Longitude();
    }
    else
    {
        s_Latitude  = Latitude_lineEdit->text();
        s_Longitude = Longitude_lineEdit->text();
    }

    s_Latitude.replace( ",", "." );
    s_Longitude.replace( ",", "." );

// ***********************************************************************************************************************

    i_ZoomLevel = ZoomLevel_spinBox->value();

    if ( MapType_1_radioButton->isChecked() == false )
        i_MapType = 3;
    else
        i_MapType = 1;

// ***********************************************************************************************************************

    if ( i_ZoomLevel < 1 )
        i_ZoomLevel = 5;

    if ( i_ZoomLevel <= 5 )
    {
        i_Height      = 1000;
        i_Width       = 1000;
    }
    else
    {
        i_Height      = 2048;
        i_Width       = 2048;
    }

// ***********************************************************************************************************************
//  app_id: Bb9y4mxzS4l7B6Zsz1Qt
//  app_code: pXgUDPOKMiUwdY-m5Z3yVA

    QString s_URL = tr( "http://image.maps.api.here.com/mia/1.6/mapview?" ) + tr( "app_id=Bb9y4mxzS4l7B6Zsz1Qt") + tr( "&" ) + tr( "app_code=pXgUDPOKMiUwdY-m5Z3yVA" )
                  + tr( "&nord" ) + QString( "&h=%1").arg( i_Height ) + QString( "&w=%1").arg( i_Width ) + QString( "&t=%1").arg( i_MapType ) + QString( "&z=%1").arg( i_ZoomLevel )
                  + QString( "&c=%1,%2" ).arg( s_Latitude ).arg( s_Longitude );

    QDesktopServices::openUrl( QUrl( s_URL ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void MainWindow::doUTMDialog()
{
    UTMDialog dialog( this );

    dialog.ReferenceEllipsoid_comboBox->setCurrentIndex( gi_utm_ReferenceEllipsoid );

    dialog.NumOfDigitsLL_spinBox->setValue( gi_utm_NumOfDigits_LL );
    dialog.NumOfDigitsLL_spinBox->setRange( 0, 10 );

    dialog.NumOfDigitsUTM_spinBox->setValue( gi_utm_NumOfDigits_UTM );
    dialog.NumOfDigitsUTM_spinBox->setRange( 0, 2 );

    if ( gi_utm_DecimalSeparator == _DOT_ )
        dialog.Dot_radioButton->setChecked( true );
    else
        dialog.Comma_radioButton->setChecked( true );

    dialog.Latitude_lineEdit->setText( "53.538780" );
    dialog.Longitude_lineEdit->setText( "8.579877" );

    dialog.ZoomLevel_spinBox->setValue( gi_utm_ZoomLevel );
    dialog.ZoomLevel_spinBox->setRange( 1, 20 );

    if ( gi_utm_MapType == 1 )
        dialog.MapType_1_radioButton->setChecked( true );
    else
        dialog.MapType_3_radioButton->setChecked( true );

    dialog.enableOKButton();
    dialog.convertLLtoUTM();

    dialog.ShowMap_pushButton->setWhatsThis( tr( "Show a map" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gi_utm_NumOfDigits_LL     = dialog.NumOfDigitsLL_spinBox->value();
        gi_utm_NumOfDigits_UTM    = dialog.NumOfDigitsUTM_spinBox->value();
        gi_utm_ZoomLevel          = dialog.ZoomLevel_spinBox->value();
        gi_utm_ReferenceEllipsoid = dialog.ReferenceEllipsoid_comboBox->currentIndex();
        gi_utm_DecimalSeparator   = _DOT_;
        gi_utm_MapType            = 1;

        if ( dialog.Dot_radioButton->isChecked() == false )
            gi_utm_DecimalSeparator = _COMMA_;

        if ( dialog.MapType_1_radioButton->isChecked() == false )
            gi_utm_MapType = 3;

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();
}
