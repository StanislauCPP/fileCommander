#ifndef THREADCLASSES_H
#define THREADCLASSES_H

#include <QThread>
#include <QFileInfo>
#include <QDir>
#include <QProgressBar>

namespace ThreadClasses {

    class CopyInformationThread : public QThread
    {
        Q_OBJECT

    public:
        CopyInformationThread(QObject *parent);

        void setParametrsForCopy(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo, QDir &dirForCopied, qint64 &fullSize);

        void setCountFullSize();

        int getCountDataPortion();

        quint64 getFullSize();

        int countDataPortion = 0;

    private:
        QFileInfo copyFileInfo;
        QString absoluteCopiedInfo;
        QDir dirForCopied;
        qint64 fullSize;

        bool checkFullSize;

        void countFullSize(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo);
        void goAndCopy(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo, QDir &dirForCopied, qint64& fullSize, int& dataSize);

        // QThread interface
    protected:
        void run() override;
    };

}

#endif // THREADCLASSES_H
