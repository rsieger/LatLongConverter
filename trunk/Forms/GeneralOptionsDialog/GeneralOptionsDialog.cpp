// ***********************************************************************************************
// *                                                                                             *
// * GeneralOptionsDialog.cpp - Sets program options                                             *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-08-07                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "GeneralOptionsDialog.h"

GeneralOptionsDialog::GeneralOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(FilenameFormat_lineEdit, SIGNAL( textChanged( QString ) ), this, SLOT(enableOKButton()));
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void GeneralOptionsDialog::enableOKButton()
{
    bool b_OK = true;

    if ( FilenameFormat_lineEdit->text().isEmpty() == true )
        b_OK = false;

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

int MainWindow::doGeneralOptionsDialog()
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    GeneralOptionsDialog dialog( this );

    dialog.FilenameFormat_lineEdit->setText( QDir::toNativeSeparators( gs_FilenameFormat ) );
    dialog.CodecInput_ComboBox->setCurrentIndex( gi_CodecInput );
    dialog.CodecOutput_ComboBox->setCurrentIndex( gi_CodecOutput );
    dialog.EOL_ComboBox->setCurrentIndex( gi_EOL );

    switch ( gi_Extension )
    {
    case _CSV_:
        dialog.CSV_radioButton->setChecked( true );
        break;
    default:
        dialog.TXT_radioButton->setChecked( true );
        break;
    }

    dialog.FilenameFormat_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.enableOKButton();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        gs_FilenameFormat = dialog.FilenameFormat_lineEdit->text().section( ".", 0, 0 );
        gi_CodecInput     = dialog.CodecInput_ComboBox->currentIndex();
        gi_CodecOutput    = dialog.CodecOutput_ComboBox->currentIndex();
        gi_EOL            = dialog.EOL_ComboBox->currentIndex();

        if ( dialog.CSV_radioButton->isChecked() )
            gi_Extension = _CSV_;
        else
            gi_Extension = _TXT_;

        gi_ActionNumber = 0;
        gsl_FilenameList.clear();

        setWTitle( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );
        showFilenameList( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList );

        i_DialogResult = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
