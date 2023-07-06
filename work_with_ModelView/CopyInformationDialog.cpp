#include "CopyInformationDialog.h"
#include "ui_CopyInformationDialog.h"

#include <QString>

CopyInformationDialog::CopyInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyInformationDialog)
{
    ui->setupUi(this);

    setFixedSize(300, 100);
    setWindowTitle("Copying information");
}

CopyInformationDialog::~CopyInformationDialog()
{
    delete ui;
}

void CopyInformationDialog::setFullSize(quint64 fullSize)
{
    ui->fullCopySizeProgressBar->setMaximum(fullSize);

    qDebug() << fullSize;
}

void CopyInformationDialog::setValue(int value)
{
    ui->fullCopySizeProgressBar->setValue(value);
}

void CopyInformationDialog::setFullCopySize(quint64 value)
{
    ui->fullCopySizeLabel->setText(QString::number(value));
}

void CopyInformationDialog::setCurrentCopySize(quint64 value)
{
    ui->currentCopySizeLabel->setText(QString::number(value));
}
