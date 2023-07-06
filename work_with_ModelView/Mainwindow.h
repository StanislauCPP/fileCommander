#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Side.h"
#include "ThreadClasses.h"

#include "CopyInformationDialog.h"
#include "CreateNewDirectoryDialog.h"

#include <QMainWindow>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QTableView>
#include <QModelIndex>
#include <QRect>
#include <QLabel>
#include <QComboBox>
#include <QStringList>
#include <QMessageBox>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /*Constructor functions*/
    void createAndSetModel();
    void getSystemDrives();
    void actionsForCopying();

    /*functions*/
    void closeOpenedAuxiablePanels();
    void setLeftSide();
    void setRightSide();
    void setLastPaths();
    void setAndShowError(QString errorMessage);

    QFileSystemModel *model;

    QTableView *currentSideTableView;
    QLineEdit *currentSideLineEdit;
    auxProperties::Side currentSide;
    QRect currentSideRegion;
    QComboBox *currentComboBox;

    QStringList lastPaths;

    QLineEdit *changeableName;

    QMessageBox *errorBox;

    auxProperties::Side prevSide;
    QModelIndex prevIndex;

    bool notClickedAction;
    bool changeableNamePanelShowed;
    bool dirButtonWindowShowed;

    int clickCount;

    CreateNewDirectoryDialog *createNewDirectoryDialog;

    ThreadClasses::CopyInformationThread *copyInformationThread;
    CopyInformationDialog *copyInformationDialog;

    QTimer *timer;

private slots:
    void onCurrentTableViewMouseButtonPressed();
    void onCurrentTableViewDoubleClicked(const QModelIndex &index);
    void onCurrentTableViewClicked(const QModelIndex &index);
    void preRename();
    void doRename();
    void onCurrentLineEditReturnPressed();
    void onCurrentComboBoxActivated(int index);

    /*ButtonPanel slots*/
    void onCopyButtonClicked();
    void onCreateNewDirButtonClicked();
    void newDirNameEntered();
    void onDeleteButtonClicked();

    /*menuTheme slot*/
    void setTheme(QAction *menuThemeAction);

    /*slot for copying*/
    void getCopyProgress();
    void copyStart();
    void copyEnd();

signals:

    void signalSetValueProgress(int);

protected:
    virtual void resizeEvent(QResizeEvent *re) override;
    virtual void keyPressEvent(QKeyEvent *ke) override;
};
#endif // MAINWINDOW_H
