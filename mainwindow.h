#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <QImage>

#include "copyfiles.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStringList video_filters;
    QStringList picture_filters;
    QStringList all_filters;
    QStringList *file_list;

    QThread copyThread;
    copyFiles *cpFfiles;

    QTimer updateTimer;

    bool is_in_process;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString selectDir(QString title);
    void getfiles(QStringList *list,const QString &dir_path);
private slots:
    void on_srcPushButton_clicked();
    void on_dstPushButton_clicked();
    void on_startBtn_clicked();
    void on_isRemove_clicked();
    void updateTimerTimeout();
    void setTotalPbRange(uint64_t size);
    void setTotalPbValue(uint64_t value);
    void setCurrentPbRange(uint64_t size);
    void setCurrentPbValue(uint64_t value);
    void copyEnd(QString result);
    void copyStop(QString result);
signals:
    void startCopyFiles(QString dst,QStringList *file_list,uint8_t classify_type,bool is_use_original_time,bool is_remove,bool ignore_same);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
