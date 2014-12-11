#ifndef LATLONGDIALOG_H
#define LATLONGDIALOG_H

#include <QDialog>

#include "ui_latlongdialog.h"

class LatLongDialog : public QDialog, public Ui::LatLongDialog
{
Q_OBJECT

public:
    LatLongDialog( QWidget *parent = 0 );

private slots:
    void clearAll();
    void showMap();

public slots:
    void convertLatLong();
    void enableOKButton();

};

#endif
