#include "copyfiles.h"

copyFiles::copyFiles(QObject *parent) : QObject(parent)
{
    is_exit = false;
    video_filters << "*.avi"<<"*.flv"<<"*.mpg"<<"*.mpeg"<<"*.mpe"<<"*.m1v"<<"*.m2v"<<"*.mpv2"<<"*.mp2v"<<"*.dat"<<"*.ts"<<"*.mp4"<<"*.m4v"<<
            "*.m4p"<<"*.m4b"<<"*.3gp"<<"*.3gpp"<<"*.3g2"<<"*.3gp2"<<"*.ogg"<<"*.mov"<<"*.rm"<<"*.rmvb"<<"*.mkv";
    picture_filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.gif" << "*.wmf" << "*.ico" << "*.psd";
    all_filters << video_filters;
    all_filters << picture_filters;
}

QStringList copyFiles::getVideoFilters()
{
    return video_filters;
}
QStringList copyFiles::getPictureFilters()
{
    return picture_filters;
}

void copyFiles::stopCopyFiles()
{
    is_exit = true;
}
uint64_t copyFiles::getCSize()
{
    return c_size;
}
uint64_t copyFiles::getIndex()
{
    return index;
}
uint64_t copyFiles::getFileSize()
{
    return file_size;
}
void copyFiles::startCopyFiles(QString dst,QStringList *file_list,uint8_t classify_type,bool is_remove,bool ignore_same)
{
    QString pic_dir = dst + "/图片";
    QString video_dir = dst + "/视频";

    QString dst_path,mk_dir,type;
    //uint64_t total_files = file_list->length();
    uint64_t pic_files = 0;
    uint64_t video_files = 0;
    uint64_t pic_files_success = 0;
    uint64_t video_files_success = 0;
    //uint64_t index = 0;
    index = 0;
    is_exit = false;
    //emit setTotalPbRange(total_files);
    foreach(QString src_file_path,*file_list)
    {
        if(is_exit)
            break;
        index++;
        //emit setTotalPbValue(index);
        QFileInfo src_file_info(src_file_path);
        type = "*." + src_file_info.suffix();
        QDateTime lt;
        if(classify_type == 0)
        {
            lt = src_file_info.birthTime().toLocalTime();
        }
        else
        {
            lt = src_file_info.lastModified().toLocalTime();
        }
        mk_dir = lt.toString("yyyy/MM");
        file_size = src_file_info.size();
        //emit setCurrentPbRange(src_file_info.size());
        //emit setCurrentPbValue(0);

        QString type_dir;
        if(picture_filters.contains(type))
        {
            type_dir = pic_dir;
            pic_files++;
        }
        else if(video_filters.contains(type))
        {
            type_dir = video_dir;
            video_files++;
        }
        else
            continue;
        dst_path = type_dir + "/" + mk_dir;
        QFileInfo dst_info(dst_path);
        QDir d;
        if(!dst_info.isDir())
            d.mkpath(dst_path);
        dst_path += "/";
        dst_path += src_file_info.fileName();

        dst_info.setFile(dst_path);
        if(dst_info.exists())
        {
//            if(picture_filters.contains(type))
//            {
//                pic_files++;
//            }
//            else if(video_filters.contains(type))
//            {
//                video_files++;
//            }
            if(ignore_same)
                continue;
            QFile f(dst_path);
            f.remove();
        }
        QFile ifile,ofile;
        //uint64_t c_size = 0;
        ifile.setFileName(src_file_path);
        if(!ifile.open(QIODevice::ReadOnly))
        {

        }
        QDataStream infile(&ifile);
        infile.setVersion(QDataStream::Qt_5_15);

        ofile.setFileName(dst_path);
        if(!ofile.open(QIODevice::WriteOnly))
        {

        }
        QDataStream outfile(&ofile);
        outfile.setVersion(QDataStream::Qt_5_15);
        char byteTemp[4096];
        int readSize = 0;
        infile.resetStatus();
        outfile.resetStatus();
        c_size = 0;
        while(!infile.atEnd())
        {
            readSize = infile.readRawData(byteTemp, 4096);
            outfile.writeRawData(byteTemp, readSize);
            c_size += readSize;
            //emit setCurrentPbValue(c_size);
        }
        ifile.close();
        ofile.close();
        if(is_remove)
            ifile.remove();
//        if(!QFile::copy(src_file_path,dst_path))
//        {
//            qDebug() << "复制失败";
//            continue;
//        }

        if(picture_filters.contains(type))
        {
            pic_files_success++;
        }
        else if(video_filters.contains(type))
        {
            video_files_success++;
        }
    }
    QString result_str;
//    result_str = QString("共%1个文件- %2 成功, %3 失败\r\n").arg(total_files).arg(index).arg(total_files - index);
//    result_str += QString("图片 %1 个- %2 成功, %3 失败\r\n").arg(pic_files).arg(pic_files_success).arg(pic_files - pic_files_success);
//    result_str += QString("视频 %1 个- %2 成功, %3 失败\r\n").arg(video_files).arg(video_files_success).arg(video_files - video_files_success);
    result_str = QString("共处理 %1个文件\r\n").arg(index);
    result_str += QString("图片 %1 个\r\n").arg(pic_files);
    result_str += QString("视频 %1 个\r\n").arg(video_files);
    if(is_exit)
        emit copyStop(result_str);
    else
        emit copyEnd(result_str);
}
