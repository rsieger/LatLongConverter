// ***********************************************************************************************
// *                                                                                             *
// * GaussKruegerDialog.cpp - Gauss-Krüger converter options dialog                              *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-05-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "convertPosition.h"

#include "GaussKruegerDialog.h"

GaussKruegerDialog::GaussKruegerDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ShowMap_pushButton, SIGNAL(clicked()), this, SLOT(showMap()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LLtoGK_pushButton, SIGNAL(clicked()), this, SLOT(convertLLtoGK()));
    connect( GKtoLL_pushButton, SIGNAL(clicked()), this, SLOT(convertGKtoLL()));

    connect( Latitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( Longitude_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( NumOfDigitsLL_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( enableOKButton() ) );
    connect( NumOfDigitsGK_spinBox, SIGNAL( valueChanged( int ) ), this, SLOT( enableOKButton() ) );
    connect( Dot_radioButton, SIGNAL( toggled( bool ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void GaussKruegerDialog::clearAll()
{
    Latitude_lineEdit->setText( "" );
    Longitude_lineEdit->setText( "" );

    GK_Streifen_lineEdit->setText( "" );
    GK_Rechts_lineEdit->setText( "" );
    GK_Hoch_lineEdit->setText( "" );

    NumOfDigitsLL_spinBox->setValue( 5 );
    NumOfDigitsLL_spinBox->setRange( 0, 10 );

    NumOfDigitsGK_spinBox->setValue( 1 );
    NumOfDigitsGK_spinBox->setRange( 0, 2 );

    Dot_radioButton->setChecked( true );

    ZoomLevel_spinBox->setValue( 5 );
    ZoomLevel_spinBox->setRange( 1, 20 );

    MapType_1_radioButton->setChecked( true );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void GaussKruegerDialog::enableOKButton()
{
    if ( ( Latitude_lineEdit->text().isEmpty() == false ) && ( Longitude_lineEdit->text().isEmpty() == false ) )
        ShowMap_pushButton->setEnabled( true );
    else
        ShowMap_pushButton->setEnabled( false );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void GaussKruegerDialog::convertLLtoGK()
{
    convertPosition *llgkc = new convertPosition();

    if ( Dot_radioButton->isChecked() == true )
        llgkc->setDecimalSeparator( _DOT_ );
    else
        llgkc->setDecimalSeparator( _COMMA_ );

    if ( ( llgkc->getMode( Latitude_lineEdit->text() ) == 1 ) || ( llgkc->getMode( Longitude_lineEdit->text() ) == 1 ) )
    {
        llgkc->setNumOfDigitsLL( 7 );
        llgkc->setNumOfDigitsGK( NumOfDigitsGK_spinBox->value() );
        llgkc->convertLLtoLL( Latitude_lineEdit->text(), Longitude_lineEdit->text() );
        llgkc->convertLLtoGK( llgkc->Latitude(), llgkc->Longitude() );
    }
    else
    {
        llgkc->setNumOfDigitsGK( NumOfDigitsGK_spinBox->value() );
        llgkc->convertLLtoGK( Latitude_lineEdit->text(), Longitude_lineEdit->text() );
    }

    if ( llgkc->DecimalSeparator() == _COMMA_ )
    {
        GK_Streifen_lineEdit->setText( QString( "%1" ).arg( llgkc->Streifen() ).replace( ".", "," ) );
        GK_Rechts_lineEdit->setText( QString( "%1" ).arg( llgkc->Rechts(), 0, 'f', NumOfDigitsGK_spinBox->value() ).replace( ".", "," ) );
        GK_Hoch_lineEdit->setText( QString( "%1" ).arg( llgkc->Hoch(), 0, 'f', NumOfDigitsGK_spinBox->value() ).replace( ".", "," ) );
    }
    else
    {
        GK_Streifen_lineEdit->setText( QString( "%1" ).arg( llgkc->Streifen() ) );
        GK_Rechts_lineEdit->setText( QString( "%1" ).arg( llgkc->Rechts(), 0, 'f', NumOfDigitsGK_spinBox->value() ) );
        GK_Hoch_lineEdit->setText( QString( "%1" ).arg( llgkc->Hoch(), 0, 'f', NumOfDigitsGK_spinBox->value() ) );
    }
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void GaussKruegerDialog::convertGKtoLL()
{
    convertPosition *gkllc = new convertPosition();

    gkllc->setNumOfDigitsLL( NumOfDigitsLL_spinBox->value() );

    if ( Dot_radioButton->isChecked() == true )
        gkllc->setDecimalSeparator( _DOT_ );
    else
        gkllc->setDecimalSeparator( _COMMA_ );

    gkllc->convertGKtoLL( GK_Streifen_lineEdit->text(), GK_Rechts_lineEdit->text(), GK_Hoch_lineEdit->text() );

    Latitude_lineEdit->setText( gkllc->Latitude() );
    Longitude_lineEdit->setText( gkllc->Longitude() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void GaussKruegerDialog::showMap()
{
    int     i_ZoomLevel   = 0;
    int     i_MapType     = 0;

    int     i_Height      = 0;
    int     i_Width       = 0;

    QString s_Latitude    = "";
    QString s_Longitude   = "";

// ***********************************************************************************************************************

    convertPosition *gkllc = new convertPosition();

    gkllc->setNumOfDigitsLL( NumOfDigitsLL_spinBox->value() );
    gkllc->setDecimalSeparator( _DOT_ );

    if ( gkllc->NumOfDigitsLL() == 0 )
    {
        gkllc->setNumOfDigitsLL( 1 );
        gkllc->convertLLtoLL( Latitude_lineEdit->text(), Longitude_lineEdit->text() );

        s_Latitude  = gkllc->Latitude();
        s_Longitude = gkllc->Longitude();
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

void MainWindow::doGaussKruegerDialog()
{
    GaussKruegerDialog dialog( this );

    dialog.NumOfDigitsLL_spinBox->setValue( gi_gk_NumOfDigits_LL );
    dialog.NumOfDigitsLL_spinBox->setRange( 0, 10 );

    dialog.NumOfDigitsGK_spinBox->setValue( gi_gk_NumOfDigits_GK );
    dialog.NumOfDigitsGK_spinBox->setRange( 0, 2 );

    if ( gi_gk_DecimalSeparator == _DOT_ )
        dialog.Dot_radioButton->setChecked( true );
    else
        dialog.Comma_radioButton->setChecked( true );

    dialog.Latitude_lineEdit->setText( "53.538780" );
    dialog.Longitude_lineEdit->setText( "8.579877" );

    dialog.ZoomLevel_spinBox->setValue( gi_gk_ZoomLevel );
    dialog.ZoomLevel_spinBox->setRange( 1, 20 );

    if ( gi_gk_MapType == 1 )
        dialog.MapType_1_radioButton->setChecked( true );
    else
        dialog.MapType_3_radioButton->setChecked( true );

    dialog.ShowMap_pushButton->setWhatsThis( tr( "Show a map" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.enableOKButton();
    dialog.convertLLtoGK();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gi_gk_NumOfDigits_LL   = dialog.NumOfDigitsLL_spinBox->value();
        gi_gk_NumOfDigits_GK   = dialog.NumOfDigitsGK_spinBox->value();
        gi_gk_ZoomLevel        = dialog.ZoomLevel_spinBox->value();
        gi_gk_DecimalSeparator = _DOT_;
        gi_gk_MapType          = 1;

        if ( dialog.Dot_radioButton->isChecked() == false )
            gi_gk_DecimalSeparator = _COMMA_;

        if ( dialog.MapType_1_radioButton->isChecked() == false )
            gi_gk_MapType = 3;

        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();
}
