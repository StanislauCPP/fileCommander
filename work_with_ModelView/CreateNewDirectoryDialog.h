#ifndef CREATENEWDIRECTORYDIALOG_H
#define CREATENEWDIRECTORYDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class CreateNewDirectoryDialog;
}

class CreateNewDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewDirectoryDialog(QWidget *parent = nullptr);
    ~CreateNewDirectoryDialog();

    QString newDirName();

private:
    Ui::CreateNewDirectoryDialog *ui;

signals:
    void newDirNameReturnPressed();
};

#endif // CREATENEWDIRECTORYDIALOG_H
