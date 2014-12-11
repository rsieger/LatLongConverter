#ifndef UTMDIALOG_H
#define UTMDIALOG_H

#include <QDialog>

#include "ui_utmdialog.h"

class UTMDialog : public QDialog, public Ui::UTMDialog
{
Q_OBJECT

public:
    UTMDialog( QWidget *parent = 0 );

private slots:
    void clearAll();
    void showMap();

public slots:
    void convertLLtoUTM();
    void convertUTMtoLL();
    void enableOKButton();

};

#endif
