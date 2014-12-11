#ifndef FINDAREADIALOG_H
#define FINDAREADIALOG_H

#include <QDialog>

#include "ui_findareadialog.h"

class FindAreaDialog : public QDialog, public Ui::FindAreaDialog
{
Q_OBJECT

public:
    FindAreaDialog( QWidget *parent = 0 );
    QString getDocumentDir();

private slots:
    void browseFilenameDialog();

public slots:
    void enableOKButton();

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
};

#endif
