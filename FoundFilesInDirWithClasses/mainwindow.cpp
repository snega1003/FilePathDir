#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <dialog.h>
#include <QIODevice>
#include <QDebug>
#include <QtGui>
#include <QString>
#include <QCryptographicHash>
#include <QMessageBox>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPrint_directory_triggered()
{
    Dialog *wd = new Dialog(this);
    wd->show();
    connect(wd,SIGNAL(FilePath(QString)),this,SLOT(ReadPath(QString)));
}

class Files {
  public:
    QFileInfo file;
    QString absolutepath;
    QString hash1;
    QString hash2;
    QString hash3;
    QString filename;
    //int size;

    template <class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & file;
        ar & absolutepath;
        ar & hash1;
        ar & hash2;
        ar & hash3;
        ar & filename;
        ar & size;
    }
};


QString ReadPath2(QString path,int level, QFile*  mFile,int k ){
    int check = 1;
    QString space;
    for(int i=1; i<= level; i++)
        space+="             ";
    QDir folder(path);
    QString buffer;
    QTextStream stream(mFile);
    QFileInfo pFile(*mFile);
    if(!folder.exists()) {
        QMessageBox msgBox;
        msgBox.setText("У вас почти получилось!");
        msgBox.exec();
        buffer = buffer + "Такой дериктории не существует. Введите путь заново. "+ '\n';
        stream << buffer;
    }

    if(k==1) {
        buffer = buffer + "Файл, описывающий дерикторию -> " + pFile.absoluteFilePath() + '\n'+ '\n'+ '\n';
        k++;
        stream << buffer;
    }
    for(QFileInfo temp : folder.entryInfoList())
    {
        if(check > 2 )
        {
            Files fileindir;
            fileindir.file = temp;
            fileindir.absolutepath = temp.absolutePath();
            fileindir.filename = temp.fileName();
            //fileindir.size = temp.size();

            QFile rem(temp.absoluteFilePath());
            rem.open(QIODevice::ReadOnly);
            QString hashMd5 = QCryptographicHash::hash(rem.readAll(), QCryptographicHash::Md5).toHex().constData();
            QString hashSha256 = QCryptographicHash::hash(rem.readAll(), QCryptographicHash::Sha256).toHex().constData();
            fileindir.hash1 = hashMd5;
            fileindir.hash2 = hashSha256;


            if(temp.isDir()){
                buffer = buffer + ReadPath2(temp.absoluteFilePath(),++level, mFile,k++);
                stream << buffer;
            }
            else {

             buffer = buffer + space + fileindir.absolutepath +"         " + fileindir.filename + /*"  size: " +fileindir.size */ +'\n'
                     + space +"     хеш файла в Md5: " + fileindir.hash1 + '\n'
                     + space +"     хеш файла в Sha256: "+ fileindir.hash2 + '\n';

             stream << buffer;
             stream << endl;
            }
             rem.close();
         }
        check++;
        //stream << buffer;
    }

    return buffer;
}

void MainWindow::ReadPath(const QString &filePath) {
    QFile mFile("mFile.txt");
    mFile.open(QIODevice::WriteOnly);
    //QFileInfo pFile(&mFile);
    ui->textEdit->setText(ReadPath2(filePath,0,&mFile,1));
    mFile.close();

}
