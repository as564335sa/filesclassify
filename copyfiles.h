#ifndef COPYFILES_H
#define COPYFILES_H

#include <QObject>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QDataStream>
#include <QDebug>
class copyFiles : public QObject
{
    Q_OBJECT
public:
    explicit copyFiles(QObject *parent = nullptr);
    QStringList video_filters;
    QStringList picture_filters;
    QStringList all_filters;

    bool is_exit = false;
    QStringList getVideoFilters();
    QStringList getPictureFilters();
    QStringList getFileList();
    void stopCopyFiles();
private slots:
    void startCopyFiles(QString dst,QStringList *file_list,uint8_t classify_type,bool is_remove,bool ignore_same);
signals:
    void setTotalPbRange(uint64_t size);
    void setTotalPbValue(uint64_t value);
    void setCurrentPbRange(uint64_t size);
    void setCurrentPbValue(uint64_t value);
    void copyEnd(QString result);
    void copyStop(QString result);
};

#endif // COPYFILES_H
