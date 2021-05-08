#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<uint64_t>("uint64_t");
    qRegisterMetaType<uint8_t>("uint8_t");
    cpFfiles = new copyFiles;
    cpFfiles->moveToThread(&copyThread);
    connect(this,SIGNAL(startCopyFiles(QString,QStringList*,uint8_t,bool,bool,bool)),cpFfiles,SLOT(startCopyFiles(QString,QStringList*,uint8_t,bool,bool,bool)));
    connect(cpFfiles,SIGNAL(setTotalPbRange(uint64_t)),this,SLOT(setTotalPbRange(uint64_t)));
    connect(cpFfiles,SIGNAL(setTotalPbValue(uint64_t)),this,SLOT(setTotalPbValue(uint64_t)));
    connect(cpFfiles,SIGNAL(setCurrentPbRange(uint64_t)),this,SLOT(setCurrentPbRange(uint64_t)));
    connect(cpFfiles,SIGNAL(setCurrentPbValue(uint64_t)),this,SLOT(setCurrentPbValue(uint64_t)));
    connect(cpFfiles,SIGNAL(copyEnd(QString)),this,SLOT(copyEnd(QString)));
    connect(cpFfiles,SIGNAL(copyStop(QString)),this,SLOT(copyStop(QString)));

    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updateTimerTimeout()));

    video_filters = cpFfiles->getVideoFilters();
    picture_filters = cpFfiles->getPictureFilters();
    all_filters = video_filters;
    all_filters << picture_filters;
    file_list = new QStringList;

    is_in_process = false;
}

MainWindow::~MainWindow()
{
    cpFfiles->stopCopyFiles();
    copyThread.quit();
    copyThread.wait();
    delete cpFfiles;
    delete file_list;
    delete ui;
}

QString MainWindow::selectDir(QString title)
{
    QString fileName;
    fileName = QFileDialog::getExistingDirectory(this,
            title, "");//QFileDialog::DontUseNativeDialog
    return fileName;
}

void MainWindow::on_srcPushButton_clicked()
{
    QString path = selectDir("选择源目录");
    if(path.isEmpty())
        return;
    ui->srcLineEdit->setText(path);
}
void MainWindow::on_dstPushButton_clicked()
{
    QString path = selectDir("选择输出目录");
    if(path.isEmpty())
        return;
    ui->dstLineEdit->setText(path);
}
void MainWindow::on_isRemove_clicked()
{
    if(ui->isRemove->isChecked())
        QMessageBox::information(this,"注意","注意：选中后将会删除原文件且不可恢复，慎用！！！");
}
void MainWindow::getfiles(QStringList *list,const QString &dir_path)
{
    //QDir dir(dir_path);
    QDir::Filters dirF_flters = QDir::Files | QDir::NoDotAndDotDot;//| QDir::Dirs |
    QDirIterator dir_iterator(dir_path,all_filters,dirF_flters,QDirIterator::Subdirectories);

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString files = file_info.absoluteFilePath();
        list->append(files);
        QCoreApplication::processEvents();
    }
}
void MainWindow::setTotalPbRange(uint64_t size)
{
    ui->progressBar->setRange(0,size);
}
void MainWindow::setTotalPbValue(uint64_t value)
{
    ui->progressBar->setValue(value);
}
void MainWindow::setCurrentPbRange(uint64_t size)
{
    ui->progressBar_2->setRange(0,size);
}
void MainWindow::setCurrentPbValue(uint64_t value)
{
    ui->progressBar_2->setValue(value);
}
void MainWindow::updateTimerTimeout()
{
    ui->progressBar->setRange(0,file_list->length());
    ui->progressBar->setValue(cpFfiles->getIndex());

    //ui->progressBar_2->setRange(0,cpFfiles->getFileSize());
    ui->progressBar_2->setValue(cpFfiles->getCSize() * 100 / cpFfiles->getFileSize());
}
void MainWindow::copyEnd(QString result)
{
    updateTimer.stop();
    ui->progressBar_2->setValue(100);
    ui->progressBar->setValue(file_list->length());
    ui->label_2->setText(result);
    is_in_process = false;
    file_list->clear();
    copyThread.quit();
    copyThread.wait();
    ui->startBtn->setText("开始整理");
    QMessageBox::information(this,"成功","整理完毕");
}
void MainWindow::copyStop(QString result)
{
    updateTimer.stop();
    ui->progressBar_2->setValue(100);
    ui->progressBar->setValue(file_list->length());
    is_in_process = false;
    ui->label_2->setText(result);
    file_list->clear();
    copyThread.quit();
    copyThread.wait();
    ui->startBtn->setText("开始整理");
    QMessageBox::information(this,"成功","手动停止");
}
void MainWindow::on_startBtn_clicked()
{
    if(is_in_process)
    {
        cpFfiles->stopCopyFiles();
        ui->startBtn->setText("等待...");
        return;
    }
    QString srcDir,dstDir;
    srcDir = ui->srcLineEdit->text();
    if(srcDir.isEmpty())
    {
        QMessageBox::information(this,"提示","请选择源路径");
        return;
    }
    dstDir = ui->dstLineEdit->text();
    if(dstDir.isEmpty())
    {
        QMessageBox::information(this,"提示","请选择输出路径");
        return;
    }
    QFileInfo fi(dstDir);
    QDir d;
    if(!fi.isDir())
    {
        d.mkpath(dstDir);
    }
    QString pic_dir = dstDir + "/图片";
    QString video_dir = dstDir + "/视频";
    d.mkpath(pic_dir);
    d.mkpath(video_dir);

    ui->label_2->setText("扫描文件...");
    getfiles(file_list,srcDir);
    if(file_list->isEmpty())
    {
        QMessageBox::information(this,"提示","未找到图片或视频文件");
        return;
    }
    ui->label_2->setText("整理中...");
    copyThread.start();
    uint8_t type = 0;
    if(ui->radioButton->isChecked())
        type = 0;
    else if(ui->radioButton_2->isChecked())
        type = 1;
    emit startCopyFiles(dstDir,file_list,type,ui->checkBox->isChecked(),ui->isRemove->isChecked(),ui->ignoreSame->isChecked());
    ui->startBtn->setText("停止");
    is_in_process = true;
    updateTimer.start(50);
}
