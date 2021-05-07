#ifndef COPYFILES_H
#define COPYFILES_H

#include <QObject>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include <QImage>
#include <QImageIOHandler>
#include <QImageReader>
//#include <QExifImageHeader>

class copyFiles : public QObject
{
    Q_OBJECT
public:
    explicit copyFiles(QObject *parent = nullptr);

    int index;
    uint64_t c_size;
    uint64_t file_size;

    QStringList video_filters;
    QStringList picture_filters;
    QStringList all_filters;

    QByteArray pic_data;
    bool is_exit = false;
    QStringList getVideoFilters();
    QStringList getPictureFilters();
    void stopCopyFiles();
    uint64_t getCSize();
    int getIndex();
    uint64_t getFileSize();

private slots:
    void startCopyFiles(QString dst,QStringList *file_list,uint8_t classify_type,bool is_use_original_time,bool is_remove,bool ignore_same);
signals:
    void setTotalPbRange(uint64_t size);
    void setTotalPbValue(uint64_t value);
    void setCurrentPbRange(uint64_t size);
    void setCurrentPbValue(uint64_t value);
    void copyEnd(QString result);
    void copyStop(QString result);
};

#endif // COPYFILES_H
