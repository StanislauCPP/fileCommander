#include "Mainwindow.h"
#include "ui_Mainwindow.h"

#include <QDir>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QTimer>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QFile>
#include <QFileInfoList>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , notClickedAction(true)
    , changeableNamePanelShowed(false)
{
    setMouseTracking(true);
    ui->setupUi(this);

    changeableName = new QLineEdit(this);
    changeableName->close();

    createAndSetModel();                        //Constructor function

    /*Set start position*/
    setLeftSide();

    getSystemDrives();                          //Constructor functions

    currentSideTableView->selectRow(0);

    /*Renaiming files and directories*/
    connect(changeableName, SIGNAL(returnPressed()), SLOT(doRename()));

    /*Creating new directories*/
    createNewDirectoryDialog = new CreateNewDirectoryDialog();
    connect(createNewDirectoryDialog, SIGNAL(newDirNameReturnPressed()), SLOT(newDirNameEntered()));

    connect(ui->menuTheme, SIGNAL(triggered(QAction*)), SLOT(setTheme(QAction*)));

    actionsForCopying();                        //Constructor functions

    errorBox = new QMessageBox(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*Constructor functions*/
void MainWindow::createAndSetModel()
{
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath(QDir::rootPath());

    ui->right_tableView->setModel(model);
    ui->left_tableView->setModel(model);
}

void MainWindow::getSystemDrives()
{
    foreach (QFileInfo FileInfo, QDir::drives())
    {
        ui->right_comboBox->addItem(FileInfo.absoluteFilePath());
        ui->left_comboBox->addItem(FileInfo.absoluteFilePath());

        lastPaths.push_back(FileInfo.absoluteFilePath());
    }
}

void MainWindow::actionsForCopying()
{
    copyInformationThread = new ThreadClasses::CopyInformationThread(this);
    copyInformationDialog = new CopyInformationDialog(this);

    timer = new QTimer(this);

    /*Copying files and directories*/
    connect(copyInformationThread, SIGNAL(started()), SLOT(copyStart()));
    connect(copyInformationThread, SIGNAL(finished()), SLOT(copyEnd()));
}
/*Constructor functions end*/

/*private functions*/
void MainWindow::closeOpenedAuxiablePanels()
{
    if(changeableNamePanelShowed)
    {
        changeableName->close();
        changeableNamePanelShowed = false;
        clickCount = 0;
    }
}

void MainWindow::setLeftSide()
{
    currentSide             = auxProperties::Side::LEFT;
    currentSideTableView    = ui->left_tableView;
    currentSideLineEdit     = ui->left_lineEdit;
    currentSideRegion       = ui->left_verticalLayout->geometry();
    currentComboBox         = ui->left_comboBox;
    clickCount              = 0;
}

void MainWindow::setRightSide()
{
    currentSide             = auxProperties::Side::RIGHT;
    currentSideTableView    = ui->right_tableView;
    currentSideLineEdit     = ui->right_lineEdit;
    currentSideRegion       = ui->right_verticalLayout->geometry();
    currentComboBox         = ui->right_comboBox;
    clickCount              = 0;
}

void MainWindow::setLastPaths()
{
    for(auto it = lastPaths.begin(); it != lastPaths.end(); ++it)
    {
        QString _lastPath = *it;
        QString _currentPath = (currentSideLineEdit->text()).toUpper();

        if(_lastPath[0] == _currentPath[0])
        {
            *it = currentSideLineEdit->text();
            break;
        }
    }
}

void MainWindow::setAndShowError(QString errorMessage)
{
    statusBar()->showMessage(errorMessage, 5000);

    errorBox->setWindowTitle("Error Box");
    errorBox->setText(errorMessage);
    errorBox->setIcon(QMessageBox::Critical);
    errorBox->show();
}
/*private functions end*/


/*SLOTS*/
void MainWindow::onCurrentTableViewMouseButtonPressed()
{
    QTableView *tableView = (QTableView*) sender();

    if(tableView->currentIndex().row() == -1)
    {
        tableView->selectRow(0);
    }
    else
        tableView->selectRow(tableView->currentIndex().row());

    if((tableView == ui->left_tableView && currentSide != auxProperties::Side::LEFT)
            || (tableView == ui->left_tableView && currentSide == auxProperties::Side::LEFT && currentSideRegion != ui->left_verticalLayout->geometry()))
    {
        setLeftSide();
    }
    else if((tableView == ui->right_tableView && currentSide != auxProperties::Side::RIGHT)
            || (tableView == ui->right_tableView && currentSide == auxProperties::Side::RIGHT && currentSideRegion != ui->right_verticalLayout->geometry()))
    {
        setRightSide();
    }

    closeOpenedAuxiablePanels();
}

void MainWindow::onCurrentTableViewDoubleClicked(const QModelIndex &index)
{
    notClickedAction = true;

    QTableView *tableView = (QTableView*) sender();
    QFileInfo _fileInfo = model->fileInfo(index);
    QString _path = model->fileInfo(index).absoluteFilePath();

    if(index.data() == "..")
    {
        _path = (model->fileInfo(index.parent())).absolutePath();
        tableView->setRootIndex(model->index(_path));

        currentSideLineEdit->setText(_path);

        setLastPaths();
    }
    else if(index.data() == ".")
    {
        tableView->setRootIndex(model->index(""));
        _path = "";

        currentSideLineEdit->setText(_path);
    }
    else if(_fileInfo.isDir())
    {
        tableView->setRootIndex(model->index(_path));

        currentSideLineEdit->setText(_path);

        setLastPaths();
    }
    else if(_fileInfo.isFile())
    {
        for(qsizetype i = 0; i < _path.size(); ++i)
        {
            if(_path[i] == '/')
            {
                _path[i] = '\\';
            }
        }

        QProcess process(this);
        process.start("explorer.exe", QStringList() << _path);
        process.waitForStarted();
        process.waitForFinished(-1);
    }

}

void MainWindow::onCurrentTableViewClicked(const QModelIndex &index)
{
    QTableView *tableView = (QTableView*) sender();
    tableView->selectRow(index.row());

    QTimer::singleShot(200, this, SLOT(preRename()));
}

void MainWindow::preRename()
{
    QModelIndex currentIndex = currentSideTableView->currentIndex();

    QFileInfo FileInfo = model->fileInfo(currentSideTableView->currentIndex());

    if(notClickedAction || prevIndex != currentIndex || prevSide != currentSide)
    {
        clickCount = 0;
        notClickedAction = false;
        prevSide = currentSide;
        prevIndex = currentIndex;

        changeableName->close();
    }

    ++clickCount;

    if(clickCount==2 && prevIndex == currentIndex && prevSide == currentSide && FileInfo.fileName() != ".." && FileInfo.fileName() != ".")
    {
        QFileInfo FileInfo = model->fileInfo(currentIndex);

        changeableName->setText(FileInfo.fileName());

        QRect currentRegion = currentSideTableView->visualRect(currentIndex);

        int horizontalPosition = currentSideRegion.left() + 40, verticalPosition = currentRegion.bottom() + 80;     /*change these parameters if add new or delete old widgets from Mainwindow*/

        changeableName->move(horizontalPosition, verticalPosition);
        changeableName->setGeometry(horizontalPosition, verticalPosition, currentRegion.width(), currentRegion.height());
        changeableName->show();
        changeableNamePanelShowed = true;
    }
}

void MainWindow::doRename()
{
    QFileInfo FileInfo = model->fileInfo(currentSideTableView->currentIndex());

    if(FileInfo.fileName() == changeableName->text())
    {
        changeableName->close();
    }
    else
    {
        QDir dir_withChangedname(FileInfo.absoluteFilePath());
        QDir dir_place = dir_withChangedname;

        dir_place.cdUp();

        QString place_path = dir_place.absolutePath();

        if (place_path[place_path.size() - 1] != '/')
        {
            place_path += '/';
        }

        if(FileInfo.isDir())
        {
            dir_withChangedname.rename(dir_withChangedname.absolutePath() + '/', (place_path + changeableName->text() + '/'));
        }
        else if(FileInfo.isFile())
        {
            dir_withChangedname.rename(dir_withChangedname.absolutePath(), (place_path + changeableName->text()));
        }

        changeableName->close();
    }
}

void MainWindow::onCurrentLineEditReturnPressed()
{
    QLineEdit *LineEdit = (QLineEdit*) sender();

    if(LineEdit == ui->left_lineEdit && currentSide != auxProperties::Side::LEFT)
    {
        setLeftSide();
    }
    else if(LineEdit == ui->right_lineEdit && currentSide != auxProperties::Side::RIGHT)
    {
        setRightSide();
    }

    QString _path = LineEdit->text();
    QDir directory(_path);

    if(directory.exists())
    {
        currentSideTableView->setRootIndex(model->index(_path));
        setLastPaths();
    }
    else
    {
        QFileInfo FileInfo;
        FileInfo = model->fileInfo(currentSideTableView->currentIndex());

        LineEdit->setText(FileInfo.absoluteFilePath());
    }
}

void MainWindow::onCurrentComboBoxActivated(int index)
{
    QComboBox *ComboBox = (QComboBox*) sender();

    if(ComboBox == ui->left_comboBox && currentSide != auxProperties::Side::LEFT)
    {
        setLeftSide();
    }
    else if(ComboBox == ui->right_comboBox && currentSide != auxProperties::Side::RIGHT)
    {
        setRightSide();
    }

    currentSideTableView->setRootIndex(model->index(lastPaths[index]));
    currentSideLineEdit->setText(lastPaths[index]);
}

/*ButtonPanel slots*/
void MainWindow::onCopyButtonClicked()
{
    closeOpenedAuxiablePanels();

    QLineEdit *anotherSideLineEdit;

    if(currentSide == auxProperties::Side::LEFT)
    {
        anotherSideLineEdit = ui->right_lineEdit;
    }
    else
    {
        anotherSideLineEdit = ui->left_lineEdit;
    }

    QFileInfo CopyFileInfo = model->fileInfo(currentSideTableView->currentIndex());
    QString absoluteCopiedInfo = anotherSideLineEdit->text() + '/' + CopyFileInfo.fileName();
    QDir dirForCopied(anotherSideLineEdit->text());

    if(dirForCopied.dirName() == ".")
    {
        setAndShowError("Can't copy to system drives");
    }
    else if(CopyFileInfo.fileName() == ".." || CopyFileInfo.fileName() == ".")
    {
        setAndShowError("Don't try copy " + CopyFileInfo.fileName());
    }
    else if(dirForCopied.absolutePath() == CopyFileInfo.absoluteFilePath())
    {
        setAndShowError("Infinite recursion. Can't copy folder to itself");
    }
    else
    {
        foreach (QFileInfo equalFileInfo, dirForCopied.entryInfoList())
        {
            if(equalFileInfo.fileName() == CopyFileInfo.fileName())
            {
                absoluteCopiedInfo = anotherSideLineEdit->text() + '/' + "_copy_" + CopyFileInfo.fileName();
                break;
            }
        }

        qint64 fullSize = 0;

        if(CopyFileInfo.isFile())
        {
            fullSize = CopyFileInfo.size();
        }
        else if(CopyFileInfo.isDir())
        {
            copyInformationThread->setParametrsForCopy(CopyFileInfo, absoluteCopiedInfo, dirForCopied, fullSize);
            copyInformationThread->setCountFullSize();
            copyInformationThread->start();
            copyInformationThread->wait();

            fullSize = copyInformationThread->getFullSize();

            copyInformationThread->quit();

        }

        copyInformationThread->setParametrsForCopy(CopyFileInfo, absoluteCopiedInfo, dirForCopied, fullSize);
        copyInformationThread->start();
    }
}

void MainWindow::onCreateNewDirButtonClicked()
{
    closeOpenedAuxiablePanels();

    if(currentSideLineEdit->text() == "")
    {
        setAndShowError("Don't try create new directory in drives");
    }
    else
    {
        createNewDirectoryDialog->show();
    }
}

void MainWindow::newDirNameEntered()
{
    QDir PathForNewDir(currentSideLineEdit->text());

    if(PathForNewDir.mkdir(currentSideLineEdit->text() + '/' + createNewDirectoryDialog->newDirName()))
    {
        statusBar()->showMessage("New directory created", 5000);
    }
    else
    {
        statusBar()->showMessage("New directory didn't created", 5000);
    }

    createNewDirectoryDialog->close();
}

void MainWindow::onDeleteButtonClicked()
{
    closeOpenedAuxiablePanels();

    QFileInfo FileInfo = model->fileInfo(currentSideTableView->currentIndex());

    if(currentSideLineEdit->text() == "")
    {
        setAndShowError("Don't try delete drives");
    }
    else if(FileInfo.fileName() == ".." || FileInfo.fileName() == ".")
    {
        setAndShowError("Don't try delete " + FileInfo.fileName());
    }
    else if(FileInfo.isFile())
    {
        QFile delFile(FileInfo.absoluteFilePath());
        delFile.remove();
    }
    else if(FileInfo.isDir())
    {
        QDir deleteDir(FileInfo.absoluteFilePath());
        deleteDir.removeRecursively();
    }
}
/*ButtonPanel slots end*/

/*menuTheme slots*/
void MainWindow::setTheme(QAction *menuThemeAction)
{
    if(menuThemeAction == ui->actionLight)
    {
        setStyleSheet("");
    }
    else if(menuThemeAction == ui->actionDark)
    {
        setStyleSheet("QHeaderView, QTableView, QLineEdit, QHeaderView::section { color: white; background-color: darkgray}");
    }
}
/*menuTheme slots end*/

/*slot for copying*/
void MainWindow::getCopyProgress()
{
    int countDataPortion = copyInformationThread->getCountDataPortion();
    quint64 currentCopySize = copyInformationThread->getFullSize()*countDataPortion/100;

    copyInformationDialog->setValue(countDataPortion);
    copyInformationDialog->setCurrentCopySize(currentCopySize);
}

void MainWindow::copyStart()
{
    copyInformationDialog->setFullCopySize(copyInformationThread->getFullSize());
    copyInformationDialog->show();

    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(getCopyProgress()));
}

void MainWindow::copyEnd()
{
    timer->stop();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(getCopyProgress()));

    copyInformationDialog->close();

    copyInformationDialog->setValue(0);
}
/*slot fo copying end*/
/*Slots end*/

/*Events*/
void MainWindow::resizeEvent(QResizeEvent *re)
{
    QWidget::resizeEvent(re);

    closeOpenedAuxiablePanels();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    QMainWindow::keyPressEvent(ke);

    switch (ke->key()) {
    case Qt::Key_F5:
        onCopyButtonClicked();
        break;

    case Qt::Key_F7:
        onCreateNewDirButtonClicked();
        break;

    case Qt::Key_F8:
        onDeleteButtonClicked();
        break;

    default:
        break;
    }
}
/*Events end*/
