#ifndef GENERALOPTIONSDIALOG_H
#define GENERALOPTIONSDIALOG_H

#include <QDialog>

#include "ui_generaloptionsdialog.h"

class GeneralOptionsDialog : public QDialog, public Ui::GeneralOptionsDialog
{
Q_OBJECT

public:
    GeneralOptionsDialog( QWidget *parent = 0 );

public slots:
    void enableOKButton();
};

#endif
