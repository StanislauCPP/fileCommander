#include "ThreadClasses.h"

ThreadClasses::CopyInformationThread::CopyInformationThread(QObject *parent) : QThread(parent)
  , checkFullSize(false)
{}

void ThreadClasses::CopyInformationThread::setParametrsForCopy(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo, QDir &dirForCopied, qint64 &fullSize)
{
    this->copyFileInfo = copyFileInfo;
    this->absoluteCopiedInfo = absoluteCopiedInfo;
    this->dirForCopied = dirForCopied;
    this->fullSize = fullSize;
}

void ThreadClasses::CopyInformationThread::setCountFullSize()
{
    checkFullSize = true;
}

int ThreadClasses::CopyInformationThread::getCountDataPortion()
{
    return countDataPortion;
}

quint64 ThreadClasses::CopyInformationThread::getFullSize()
{
    return fullSize;
}

void ThreadClasses::CopyInformationThread::run()
{
    if(checkFullSize)
    {
        countFullSize(copyFileInfo, absoluteCopiedInfo);

        checkFullSize = false;
    }
    else
    {
        countDataPortion = 0;

        goAndCopy(copyFileInfo, absoluteCopiedInfo, dirForCopied, fullSize, countDataPortion);

        countDataPortion = 0;
    }
}

void ThreadClasses::CopyInformationThread::countFullSize(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo)
{
    if(copyFileInfo.isFile())
    {
        this->fullSize += copyFileInfo.size();
    }
    else if(copyFileInfo.isDir())
    {
        QDir copyDir(copyFileInfo.absoluteFilePath());

        if(!copyDir.isEmpty())
        {
            foreach (copyFileInfo, copyDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst))
            {
                absoluteCopiedInfo = dirForCopied.absolutePath() + '/' + copyFileInfo.fileName();
                countFullSize(copyFileInfo, absoluteCopiedInfo);
            }
        }
    }
}

void ThreadClasses::CopyInformationThread::goAndCopy(QFileInfo &copyFileInfo, QString &absoluteCopiedInfo, QDir &dirForCopied, qint64 &fullSize, int &countDataPortion)
{
    if(copyFileInfo.isFile())
    {
        QFile copiableFile(copyFileInfo.filePath());
        QFile copiedFile(absoluteCopiedInfo);

        copiableFile.open(QIODevice::ReadOnly);
        copiedFile.open(QIODevice::ReadWrite);

        QByteArray data;

        quint64 dataPortion = fullSize/100;

        while(!copiableFile.atEnd())
        {
            data = copiableFile.read(dataPortion);

            copiedFile.write(data);

            ++countDataPortion;

        }

        copiableFile.close();
        copiedFile.close();
    }
    else if(copyFileInfo.isDir())
    {
        QDir copyDir(copyFileInfo.absoluteFilePath());

        dirForCopied.mkdir(absoluteCopiedInfo);

        if(!copyDir.isEmpty())
        {
            dirForCopied.cd(absoluteCopiedInfo);

            foreach (copyFileInfo, copyDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst))
            {
                absoluteCopiedInfo = dirForCopied.absolutePath() + '/' + copyFileInfo.fileName();
                goAndCopy(copyFileInfo, absoluteCopiedInfo, dirForCopied, fullSize, countDataPortion);
            }

            dirForCopied.cdUp();
        }
    }
}
