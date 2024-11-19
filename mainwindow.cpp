#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QThread>
#include <QTimer>
#include <QPointer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_startComparingButton_clicked()
{
    ui->startComparingButton->setEnabled(false);
    bool ifSame = false;
    ui->outputTextBrowser->clear();
    this->repaint(); // 立即刷新
    //QThread::msleep(1000); // 毫秒级

    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");

    QByteArray buffer1, buffer2;
    const qint64 bufferSize = 4096; // 每次读取 4KB
    long frequency = 0;

    QString pathA = ui->pathALineEdit->text();
    QString pathB = ui->pathBLineEdit->text();

    //pathA = R"(C:\工作区\Workspace\test.txt)";
    //pathB = R"(C:\工作区\Workspace\test2.txt)";

    //ui->pathALineEdit->setText(pathA);
    QFile fileA1(pathA);
    QFile fileB1(pathB);

    if (fileA1.exists()&&fileB1.exists()||true) {
        //qDebug() << "找到文件:" << pathA;

        // 尝试打开两个文件
        if (!fileA1.open(QIODevice::ReadOnly)) {
            qWarning() << "无法打开文件1:" << pathA;
            ui->outputTextBrowser->append("无法打开文件A:"+pathA);
            this->repaint(); // 立即刷新
            goto end1;

        }
        if (!fileB1.open(QIODevice::ReadOnly)) {
            qWarning() << "无法打开文件2:" << pathB;
            ui->outputTextBrowser->append("无法打开文件B:"+pathB);
            this->repaint(); // 立即刷新
            goto end1;

        }

        // 检查文件大小是否一致
        if (fileA1.size() != fileB1.size()) {
            qDebug() << "文件大小不一致";
            ui->outputTextBrowser->append("文件大小不一致");
            this->repaint(); // 立即刷新
            goto end1;
        }


        // 对比文件内容

        while (!fileA1.atEnd() && !fileB1.atEnd()) {
            buffer1 = fileA1.read(bufferSize);
            buffer2 = fileB1.read(bufferSize);


            qDebug() << 1.0*frequency*bufferSize/fileA1.size();
            int schedule = qFloor(1.0*frequency*bufferSize/fileA1.size()*100);
            if(schedule>100){
                schedule = 100;
            }

            qDebug() << schedule;
            ui->progressBar->setValue(schedule);

            if (buffer1 != buffer2) {
                qDebug() << "文件不一致";
                ui->outputTextBrowser->append("文件不一致");
                ui->progressBar->setValue(100);
                goto JumpOutComparison;
            }
            frequency++;
            QThread::msleep(100); // 毫秒级
        }
        // 如果文件完全相同，返回 true
        ui->progressBar->setValue(100);
        qDebug() << "文件一致" << fileA1.size() << ":"<<frequency;
        ifSame = true;





//FileInconsistency:;

        JumpOutComparison:;



        qDebug() << "ok1122";

        if (ifSame){
            QString outputHtml = ui->outputTextBrowser->toHtml();
            ui->outputTextBrowser->setHtml("<b><span style='color:green;font-size:24px;'>文件相同</span></b>"+timeStr+"<br/>"+outputHtml);
        }else{
            QString outputHtml = ui->outputTextBrowser->toHtml();
            ui->outputTextBrowser->setHtml("<b><span style='color:red;font-size:24px;'>文件不相同</span></b>"+timeStr+"<br/>"+outputHtml);
        }

end1:;

        fileA1.close();
        fileB1.close();



    }else {

        if(!fileA1.exists()){
            qDebug() << "文件A不存在:" << pathA;
            ui->outputTextBrowser->append("文件A不存在:"+pathA);
        }
        if(!fileB1.exists()){
            qDebug() << "文件B不存在:" << pathB;
            ui->outputTextBrowser->append("文件B不存在:"+pathB);
        }

    }



    QPointer<QPushButton> button = ui->startComparingButton;
    QTimer::singleShot(200, [button]() {
        if (button) {
            button->setEnabled(true);
            qDebug() << "工作完成！";
        }
    });



    //ui->startComparingButton->setEnabled(true);
}


void MainWindow::on_pushButton_clicked()
{
    ui->outputTextBrowser->append("<b>123</b>");
    this->repaint(); // 立即刷新
    QThread::msleep(1000); // 毫秒级
    //ui->progressBar->setValue(50);
    QThread::msleep(1000); // 毫秒级
    ui->progressBar->setValue(90);
    ui->outputTextBrowser->append("<b>456</b>");

}

