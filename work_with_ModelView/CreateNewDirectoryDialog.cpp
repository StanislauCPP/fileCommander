#include "CreateNewDirectoryDialog.h"
#include "ui_CreateNewDirectoryDialog.h"

#include <QSizePolicy>

CreateNewDirectoryDialog::CreateNewDirectoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewDirectoryDialog)
{
    ui->setupUi(this);

    setModal(true);
    setFixedSize(300, 80);
    setWindowTitle("Create new directory");

    connect(ui->newDirName, SIGNAL(returnPressed()), SIGNAL(newDirNameReturnPressed()));
}

CreateNewDirectoryDialog::~CreateNewDirectoryDialog()
{
    delete ui;
}

QString CreateNewDirectoryDialog::newDirName()
{
    return ui->newDirName->text();
}
