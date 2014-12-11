#ifndef LATLONGOPTIONSDIALOG_H
#define LATLONGOPTIONSDIALOG_H

#include <QDialog>

#include "ui_latlongoptionsdialog.h"

class LatLongOptionsDialog : public QDialog, public Ui::LatLongOptionsDialog
{
Q_OBJECT

public:
    LatLongOptionsDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
