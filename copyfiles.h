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

    uint64_t index;
    uint64_t c_size;
    uint64_t file_size;

    QStringList video_filters;
    QStringList picture_filters;
    QStringList all_filters;

    bool is_exit = false;
    QStringList getVideoFilters();
    QStringList getPictureFilters();
    void stopCopyFiles();
    uint64_t getCSize();
    uint64_t getIndex();
    uint64_t getFileSize();

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
