#ifndef COPYINFORMATIONDIALOG_H
#define COPYINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CopyInformationDialog;
}

class CopyInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyInformationDialog(QWidget *parent = nullptr);
    ~CopyInformationDialog();

    void setFullSize(quint64 fullSize);

private:
    Ui::CopyInformationDialog *ui;

public slots:
    void setValue(int value);
    void setFullCopySize(quint64 value);
    void setCurrentCopySize(quint64 value);
};

#endif // COPYINFORMATIONDIALOG_H
