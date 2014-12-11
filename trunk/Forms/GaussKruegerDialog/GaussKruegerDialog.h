#ifndef GAUSSKRUEGERDIALOG_H
#define GAUSSKRUEGERDIALOG_H

#include <QDialog>

#include "ui_gausskruegerdialog.h"

class GaussKruegerDialog : public QDialog, public Ui::GaussKruegerDialog
{
Q_OBJECT

public:
    GaussKruegerDialog( QWidget *parent = 0 );

private slots:
    void clearAll();
    void showMap();

public slots:
    void convertLLtoGK();
    void convertGKtoLL();
    void enableOKButton();

};

#endif
