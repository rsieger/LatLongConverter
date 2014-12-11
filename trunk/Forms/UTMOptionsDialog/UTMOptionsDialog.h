#ifndef UTMOPTIONSDIALOG_H
#define UTMOPTIONSDIALOG_H

#include <QDialog>

#include "ui_utmoptionsdialog.h"

class UTMOptionsDialog : public QDialog, public Ui::UTMOptionsDialog
{
Q_OBJECT

public:
    UTMOptionsDialog( QWidget *parent = 0 );

private slots:
    void clearAll();

public slots:
    void enableOKButton();

};

#endif
