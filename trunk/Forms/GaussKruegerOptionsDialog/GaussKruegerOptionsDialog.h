#ifndef GAUSSKRUEGEROPTIONSDIALOG_H
#define GAUSSKRUEGEROPTIONSDIALOG_H

#include <QDialog>

#include "ui_gausskruegeroptionsdialog.h"

class GaussKruegerOptionsDialog : public QDialog, public Ui::GaussKruegerOptionsDialog
{
Q_OBJECT

public:
    GaussKruegerOptionsDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
