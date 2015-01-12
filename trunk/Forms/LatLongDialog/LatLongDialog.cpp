// ***********************************************************************************************
// *                                                                                             *
// * LatLongDialog.cpp - Latitude/Longitude converter options dialog                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-05-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "convertPosition.h"
#include "LatLongDialog.h"

LatLongDialog::LatLongDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ShowMap_pushButton, SIGNAL(clicked()), this, SLOT(showMap()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( Latitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( Longitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumOfDigits_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( enableOKButton() ) );
    connect( Dot_radioButton, SIGNAL( toggled( bool ) ), this, SLOT( enableOKButton() ) );

    connect( Latitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( convertLatLong() ) );
    connect( Longitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( convertLatLong() ) );
    connect( NumOfDigits_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( convertLatLong() ) );
    connect( Dot_radioButton, SIGNAL( toggled( bool ) ), this, SLOT( convertLatLong() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void LatLongDialog::clearAll()
{
    Latitude_lineEdit->setText( "" );
    Longitude_lineEdit->setText( "" );

    LatitudeResult_lineEdit->setText( "" );
    LongitudeResult_lineEdit->setText( "" );

    NumOfDigits_spinBox->setValue( 5 );
    NumOfDigits_spinBox->setRange( 0, 10 );

    Dot_radioButton->setChecked( true );

    ZoomLevel_spinBox->setValue( 5 );
    ZoomLevel_spinBox->setRange( 1, 20 );

    MapType_1_radioButton->setChecked( true );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void LatLongDialog::enableOKButton()
{
    if ( ( Latitude_lineEdit->text().isEmpty() == false ) && ( Longitude_lineEdit->text().isEmpty() == false ) )
        ShowMap_pushButton->setEnabled( true );
    else
        ShowMap_pushButton->setEnabled( false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void LatLongDialog::convertLatLong()
{
    convertPosition *llc = new convertPosition();

    llc->setNumOfDigitsLL( NumOfDigits_spinBox->value() );

    if ( Dot_radioButton->isChecked() == true )
        llc->setDecimalSeparator( _DOT_ );
    else
        llc->setDecimalSeparator( _COMMA_ );

    llc->convertLLtoLL( Latitude_lineEdit->text(), Longitude_lineEdit->text() );

    LatitudeResult_lineEdit->setText( llc->Latitude() );
    LongitudeResult_lineEdit->setText( llc->Longitude() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void LatLongDialog::showMap()
{
    int     i_ZoomLevel   = 0;
    int     i_MapType     = 0;

    int     i_Height      = 0;
    int     i_Width       = 0;

    QString s_Latitude    = "";
    QString s_Longitude   = "";

// ***********************************************************************************************************************

    convertLatLong();

    s_Latitude  = LatitudeResult_lineEdit->text();
    s_Longitude = LongitudeResult_lineEdit->text();

    if ( ( s_Latitude.endsWith( "n", Qt::CaseInsensitive ) == true ) || ( s_Latitude.endsWith( "s", Qt::CaseInsensitive ) == true ) )
        s_Latitude = Latitude_lineEdit->text();

    if ( ( s_Longitude.endsWith( "e", Qt::CaseInsensitive ) == true ) || ( s_Longitude.endsWith( "w", Qt::CaseInsensitive ) == true ) )
        s_Longitude = Longitude_lineEdit->text();

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

void MainWindow::doLatLongDialog()
{
    LatLongDialog dialog( this );

    dialog.NumOfDigits_spinBox->setValue( gi_ll_NumOfDigits );
    dialog.NumOfDigits_spinBox->setRange( 0, 10 );

    if ( gi_ll_DecimalSeparator == _DOT_ )
        dialog.Dot_radioButton->setChecked( true );
    else
        dialog.Comma_radioButton->setChecked( true );

    dialog.Latitude_lineEdit->setText( tr( "53°32'19.61'' N" ) );
    dialog.Longitude_lineEdit->setText( tr( "8°34'47.56'' E" ) );

    dialog.ZoomLevel_spinBox->setValue( gi_ll_ZoomLevel );
    dialog.ZoomLevel_spinBox->setRange( 1, 20 );

    if ( gi_ll_MapType == 1 )
        dialog.MapType_1_radioButton->setChecked( true );
    else
        dialog.MapType_3_radioButton->setChecked( true );

    dialog.ShowMap_pushButton->setWhatsThis( tr( "Show a map" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.enableOKButton();
    dialog.convertLatLong();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gi_ll_NumOfDigits      = dialog.NumOfDigits_spinBox->value();
        gi_ll_ZoomLevel        = dialog.ZoomLevel_spinBox->value();
        gi_ll_DecimalSeparator = _DOT_;
        gi_ll_MapType          = 1;

        if ( dialog.Dot_radioButton->isChecked() == false )
            gi_ll_DecimalSeparator = _COMMA_;

        if ( dialog.MapType_1_radioButton->isChecked() == false )
            gi_ll_MapType = 3;

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();
}
