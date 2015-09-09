// ***********************************************************************************************
// *                                                                                             *
// * UTMOptionsDialog.cpp - UTM converter options dialog                                         *
// *                                                                                             *
// * Dr. Rainer Sieger - 2012-08-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "UTMOptionsDialog.h"

UTMOptionsDialog::UTMOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    LatitudeColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );
    LongitudeColumn_lineEdit->setValidator( new QIntValidator( 1, 9999, this ) );

    connect(ClearAll_pushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect( LatitudeColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
    connect( LongitudeColumn_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( enableOKButton() ) );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void UTMOptionsDialog::clearAll()
{
    LatitudeColumn_lineEdit->setText( "" );
    LongitudeColumn_lineEdit->setText( "" );
    NorthingColumn_lineEdit->setText( "" );
    EastingColumn_lineEdit->setText( "" );
    ZoneColumn_lineEdit->setText( "" );

    ReferenceEllipsoid_comboBox_1->setCurrentIndex( 22 );
    ReferenceEllipsoid_comboBox_2->setCurrentIndex( 22 );

    NumOfDigits_spinBox->setValue( 4 );

    Dot_radioButton->setChecked( true );

    enableOKButton();
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void UTMOptionsDialog::enableOKButton()
{
    QList<int>  testList;

    bool b_OK = false;

    testList.append( LatitudeColumn_lineEdit->text().toInt() );         // 0
    testList.append( LongitudeColumn_lineEdit->text().toInt() );        // 1

    if ( testList.at( 0 ) < 1 )  LatitudeColumn_lineEdit->setText( "" );
    if ( testList.at( 1 ) < 1 )  LongitudeColumn_lineEdit->setText( "" );

    if ( ( testList.at( 0 ) > 0 ) && ( testList.at( 1 ) > 0 ) )         // Latitude > 0 and Longitude > 0
        b_OK = true;

    if ( b_OK == true )
    {
        for ( int i=0; i<testList.count(); i++ )
        {
            if ( testList.at( i ) > 0 )
            {
                for ( int j=0; j<testList.count(); j++ )
                {
                    if ( ( i != j ) && ( testList.at( i ) == testList.at( j ) ) )
                         b_OK = false;
                }
            }
        }
    }

    if ( b_OK == true )
    {
        OK_pushButton->setEnabled( true );
        OK_pushButton->setDefault( true );
    }
    else
    {
        OK_pushButton->setEnabled( false );
        Cancel_pushButton->setDefault( true );
    }
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doUTMOptionsDialog( const int mode, int &i_LatitudeColumn, int &i_LongitudeColumn, int &i_NorthingColumn, int &i_EastingColumn, int &i_ZoneColumn, int &i_ReferenceEllipsoid, int &i_NumOfDigits, int &i_DecimalSeparator, bool &b_DeleteInputFile )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    UTMOptionsDialog dialog( this );

    dialog.LatitudeColumn_lineEdit->setText( QString( "%1" ).arg( i_LatitudeColumn ) );
    dialog.LongitudeColumn_lineEdit->setText( QString( "%1" ).arg( i_LongitudeColumn ) );

    dialog.NorthingColumn_lineEdit->setText( QString( "%1" ).arg( i_NorthingColumn ) );
    dialog.EastingColumn_lineEdit->setText( QString( "%1" ).arg( i_EastingColumn ) );
    dialog.ZoneColumn_lineEdit->setText( QString( "%1" ).arg( i_ZoneColumn ) );

    dialog.ReferenceEllipsoid_comboBox_1->setCurrentIndex( i_ReferenceEllipsoid );
    dialog.ReferenceEllipsoid_comboBox_2->setCurrentIndex( i_ReferenceEllipsoid );

    if ( i_DecimalSeparator == _DOT_ )
        dialog.Dot_radioButton->setChecked( true );
    else
        dialog.Comma_radioButton->setChecked( true );

    dialog.DeleteInputFile_checkBox->setChecked( b_DeleteInputFile );

    if ( gsl_FilenameList.count() > 1 )
        dialog.DeleteInputFile_checkBox->setText( tr( "Delete input files, be careful" ) );

    switch ( mode )
    {
    case _LLUTM_:
        dialog.setWindowTitle( tr( "Latitude/Longitude to UTM" ) );
        dialog.NumOfDigits_spinBox->setValue( i_NumOfDigits );
        dialog.NumOfDigits_spinBox->setRange( 0, 10 );
        dialog.UTM_groupBox->hide();
        dialog.ReferenceEllipsoid_groupBox_2->hide();
        break;
    case _UTMLL_:
        dialog.setWindowTitle( tr( "UTM to Latitude/Longitude" ) );
        dialog.NumOfDigits_spinBox->setValue( i_NumOfDigits );
        dialog.NumOfDigits_spinBox->setRange( 0, 10 );
        dialog.Geodetic_groupBox->hide();
        dialog.ReferenceEllipsoid_groupBox_1->hide();
        break;
    }

    dialog.OK_pushButton->setWhatsThis( tr( "Close dialog" ) );
    dialog.Cancel_pushButton->setWhatsThis( tr( "Cancel dialog" ) );

    dialog.enableOKButton();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        switch ( mode )
        {
        case _LLUTM_:
            i_LatitudeColumn     = dialog.LatitudeColumn_lineEdit->text().toInt();
            i_LongitudeColumn    = dialog.LongitudeColumn_lineEdit->text().toInt();
            i_ReferenceEllipsoid = dialog.ReferenceEllipsoid_comboBox_1->currentIndex();
            break;
        case _UTMLL_:
            i_NorthingColumn     = dialog.NorthingColumn_lineEdit->text().toInt();
            i_EastingColumn      = dialog.EastingColumn_lineEdit->text().toInt();
            i_ZoneColumn         = dialog.ZoneColumn_lineEdit->text().toInt();
            i_ReferenceEllipsoid = dialog.ReferenceEllipsoid_comboBox_2->currentIndex();
            break;
        }

        i_NumOfDigits = dialog.NumOfDigits_spinBox->value();

        if ( dialog.Dot_radioButton->isChecked() == true )
            i_DecimalSeparator = _DOT_;
        else
            i_DecimalSeparator = _COMMA_;

        b_DeleteInputFile  = dialog.DeleteInputFile_checkBox->isChecked();

        i_DialogResult     = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
