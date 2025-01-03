#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "draglineedit.h"

#include <QFile>
#include <QThread>
#include <QTimer>
#include <QPointer>
#include <QDateTime>
#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , aboutWindow(nullptr) // 初始化新窗体指针为 nullptr
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_startComparingButton_clicked()
{
    QElapsedTimer timer;
    timer.start(); // 开始计时
    ui->startComparingButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->pathALineEdit->setEnabled(false);
    ui->pathBLineEdit->setEnabled(false);
    ui->startComparingButton->setStyleSheet("background-color: #99a3a4;");
    ui->startComparingButton->setText("正在对比...");
    ui->pathALineEdit->setStyleSheet("background-color: #e5e7e9;");
    ui->pathBLineEdit->setStyleSheet("background-color: #e5e7e9;");
    ui->infoLabel->setText("");
    ui->infoLabel->setStyleSheet("color:#000000;");
    ui->resetButton->setStyleSheet("background-color: #99a3a4;");

    this->repaint(); // 立即刷新

    bool isFileSame1 = false;
    bool isFileSame2 = true;
    ui->outputTextBrowser->clear();
    this->repaint(); // 立即刷新
    //QThread::msleep(1000); // 毫秒级

    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString timeStr = dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");

    QByteArray bufferA1, bufferB1;
    const qint64 bufferSize = 8192; // 每次读取 8KB
    long frequency = 0;

    QString pathA = ui->pathALineEdit->text();
    QString pathB = ui->pathBLineEdit->text();

    //ui->pathALineEdit->setText(pathA);
    QFile fileA1(pathA);
    QFile fileB1(pathB);

    QCryptographicHash fileA1Hash(QCryptographicHash::Sha3_512);
    QCryptographicHash fileB1Hash(QCryptographicHash::Sha3_512);
    QCryptographicHash fileA2Hash(QCryptographicHash::Sha3_512);
    QCryptographicHash fileB2Hash(QCryptographicHash::Sha3_512);


    QString fileA1HashString="";
    QString fileB1HashString="";
    QString outString="";
    QString outString2="";

    if (fileA1.exists()&&fileB1.exists()) {
        //qDebug() << "找到文件:" << pathA;

        // 尝试打开两个文件
        if (!fileA1.open(QIODevice::ReadOnly)) {

            ui->outputTextBrowser->append("无法打开文件A:"+pathA);
            this->repaint(); // 立即刷新
            goto end1;

        }
        if (!fileB1.open(QIODevice::ReadOnly)) {

            ui->outputTextBrowser->append("无法打开文件B:"+pathB);
            this->repaint(); // 立即刷新
            goto end1;

        }

        numberOfComparisons++;

        // 检查文件大小是否一致
        if (fileA1.size() != fileB1.size()) {
            isFileSame2=false;
            ui->outputTextBrowser->append("文件大小不一致");
            ui->outputTextBrowser->append("文件A大小="+QString::number(fileA1.size())+"字节");
            ui->outputTextBrowser->append("文件B大小="+QString::number(fileB1.size())+"字节");


            QByteArray bufferA2, bufferB2;
            //const qint64 bufferSize = 8192; // 每次读取 8KB
            long frequencyA2 = 0;
            long frequencyB2 = 0;
            QString fileA2HashString="";
            QString fileB2HashString="";


            while (!fileA1.atEnd()){

                bufferA2 = fileA1.read(bufferSize);
                fileA2Hash.addData(bufferA2);
                double scheduleDouble = 1.0*frequencyA2*bufferSize/fileA1.size()*100;
                int scheduleInt = qFloor(scheduleDouble);
                if(scheduleInt>100){
                    scheduleInt = 100;
                }

                ui->infoLabel->setText("(1/2)正在计算文件A哈希值..."+QString::number(scheduleDouble, 'f', 2)+"%"); // 保留两位小数;
                ui->progressBar->setValue(scheduleInt);
                frequencyA2++;

            }
            fileA2HashString = fileA2Hash.result().toHex();
            ui->infoLabel->setText("(1/2)正在计算文件A哈希值...100%");
            ui->progressBar->setValue(100);

            //QThread::msleep(3000); // 毫秒级

            outString.clear();
            outString+="<table><tr><td>文件A SHA3-512</td><td>=</td><td>"+fileA2HashString+"</td></tr></table>";
            ui->outputTextBrowser->append(outString);

            this->repaint(); // 立即刷新

            while (!fileB1.atEnd()){

                bufferB2 = fileB1.read(bufferSize);
                fileB2Hash.addData(bufferB2);
                double scheduleDouble = 1.0*frequencyB2*bufferSize/fileA1.size()*100;
                int scheduleInt = qFloor(scheduleDouble);
                if(scheduleInt>100){
                    scheduleInt = 100;
                }

                ui->infoLabel->setText("(2/2)正在计算文件B哈希值..."+QString::number(scheduleDouble, 'f', 2)+"%"); // 保留两位小数;
                ui->progressBar->setValue(scheduleInt);
                frequencyB2++;

            }
            fileB2HashString = fileB2Hash.result().toHex();

            ui->infoLabel->setText("(2/2)正在计算文件B哈希值...100%");
            ui->progressBar->setValue(100);

            outString.clear();
            outString+="<table><tr><td>文件B SHA3-512</td><td>=</td><td>"+fileB2HashString+"</td></tr></table>";
            ui->outputTextBrowser->append(outString);

            this->repaint(); // 立即刷新

            if(fileA2HashString==fileB2HashString){
                ui->outputTextBrowser->append("<b><span style='color:red;font-size:24px;'>哈希碰撞！</span></b>");

            }




            ui->infoLabel->setText("完成！");
            this->repaint(); // 立即刷新
            goto JumpOutComparison;
        }


        ui->outputTextBrowser->append("文件大小="+QString::number(fileA1.size())+"字节");

        // 对比文件内容

        while (!fileA1.atEnd() && !fileB1.atEnd()) {
            bufferA1 = fileA1.read(bufferSize);
            bufferB1 = fileB1.read(bufferSize);

            fileA1Hash.addData(bufferA1);
            fileB1Hash.addData(bufferB1);



            double scheduleDouble = 1.0*frequency*bufferSize/fileA1.size()*100;
            int scheduleInt = qFloor(scheduleDouble);
            if(scheduleInt>100){
                scheduleInt = 100;
            }

            //qDebug() << scheduleInt;
            ui->infoLabel->setText("正在对比..."+QString::number(scheduleDouble, 'f', 2)+"%"); // 保留两位小数;
            ui->progressBar->setValue(scheduleInt);

            if (isFileSame2&&(bufferA1 != bufferB1)) {
                qDebug() << "文件不一致";
                isFileSame1=false;
                isFileSame2=false;
                ui->outputTextBrowser->append("文件不一致");

                //goto JumpOutComparison;
            }

            frequency++;
            //QThread::msleep(1000); // 毫秒级
        }
        // 对比完成！
        ui->progressBar->setValue(100);
        //qDebug() << "文件一致" << fileA1.size() << ":"<<frequency;

        fileA1HashString = fileA1Hash.result().toHex();
        fileB1HashString = fileB1Hash.result().toHex();
        if(isFileSame2){


            if(fileA1HashString==fileB1HashString){
                ui->outputTextBrowser->append("文件一致");

                outString.clear();
                outString+="<table><tr><td>SHA3-512</td><td>=</td><td>"+fileA1HashString+"</td></tr></table>";
                ui->outputTextBrowser->append(outString);
            }else{
                outString.clear();


                outString+="<br/>";
                outString+="<table>";
                outString+="<tr>";
                outString+="<td>文件A SHA3-512</td><td>=</td><td>"+fileA1HashString+"</td>";
                outString+="</tr>";
                outString+="<tr>";
                outString+="<td>文件B SHA3-512</td><td>=</td><td>"+fileB1HashString+"</td>";
                outString+="</tr>";
                outString+="</table>";
                ui->outputTextBrowser->append(outString);


                outString2.clear();
                outString2=ui->outputTextBrowser->toHtml();
                outString2="<b><span style='color:red;font-size:24px;'>严重错误：文件一致但哈希值不同！</span></b><span style='color:blue;'>("+QString::number(numberOfComparisons)+")</span>&nbsp;"+timeStr+"<br/>"+outString2;
                ui->outputTextBrowser->setHtml(outString2);
                goto end1;

            }

        }else{

            if(fileA1HashString==fileB1HashString){
                ui->outputTextBrowser->append("<b><span style='color:red;font-size:24px;'>哈希碰撞！</span></b>");
                outString.clear();
                outString+="<table><tr><td>SHA3-512</td><td>=</td><td>"+fileA1HashString+"</td></tr></table>";
                ui->outputTextBrowser->append(outString);
            }else{
                outString.clear();


                outString+="<br/>";
                outString+="<table>";
                outString+="<tr>";
                outString+="<td>文件A SHA3-512</td><td>=</td><td>"+fileA1HashString+"</td>";
                outString+="</tr>";
                outString+="<tr>";
                outString+="<td>文件B SHA3-512</td><td>=</td><td>"+fileB1HashString+"</td>";
                outString+="</tr>";
                outString+="</table>";
                ui->outputTextBrowser->append(outString);


            }


        }

        isFileSame1 = true;
        ui->infoLabel->setText("完成！");
        this->repaint(); // 立即刷新





        //FileInconsistency:;

JumpOutComparison:;


        //qDebug() << "ok1122";

        if (isFileSame2){
            QString outputHtml = ui->outputTextBrowser->toHtml();
            ui->outputTextBrowser->setHtml("<b><span style='color:green;font-size:24px;'>文件相同</span></b><span style='color:blue;'>("+QString::number(numberOfComparisons)+")</span>&nbsp;"+timeStr+"<br/>"+outputHtml);
        }else{
            QString outputHtml = ui->outputTextBrowser->toHtml();
            ui->outputTextBrowser->setHtml("<b><span style='color:red;font-size:24px;'>文件不相同</span></b><span style='color:blue;'>("+QString::number(numberOfComparisons)+")</span>&nbsp;"+timeStr+"<br/>"+outputHtml);
        }

end1:;

        fileA1.close();
        fileB1.close();



    }else {


        if(!fileA1.exists()){
            ui->outputTextBrowser->append("文件A不存在:"+pathA);
        }else{
            ui->outputTextBrowser->append("文件A大小="+QString::number(fileA1.size())+"字节");

        }



        if(!fileB1.exists()){
            ui->outputTextBrowser->append("文件B不存在:"+pathB);
        }else{
            ui->outputTextBrowser->append("文件B大小="+QString::number(fileB1.size())+"字节");

        }

        if(fileA1.exists()){

            long frequencyA3 = 0;
            //long frequencyB2 = 0;

            if (!fileA1.open(QIODevice::ReadOnly)) {
                qWarning() << "无法打开文件：" << pathA;
                //return false;
            }

            QCryptographicHash fileA3Hash(QCryptographicHash::Sha3_512);

            // 按块读取文件并更新哈希值
            while (!fileA1.atEnd()) {
                QByteArray data = fileA1.read(bufferSize); // 每次读取 bufferSize KB
                fileA3Hash.addData(data);

                double scheduleDouble = 1.0*frequencyA3*bufferSize/fileA1.size()*100;
                int scheduleInt = qFloor(scheduleDouble);
                if(scheduleInt>100){
                    scheduleInt = 100;
                }

                ui->infoLabel->setText("正在计算文件A哈希值..."+QString::number(scheduleDouble, 'f', 2)+"%"); // 保留两位小数;
                ui->progressBar->setValue(scheduleInt);
                frequencyA3++;
            }

            // 关闭文件
            fileA1.close();

            // 哈希值的十六进制表示
            QString hashString = fileA3Hash.result().toHex();

            ui->infoLabel->setText("正在计算文件A哈希值...100%"); // 保留两位小数;
            ui->progressBar->setValue(100);

            ui->outputTextBrowser->append("<table><tr><td>文件A SHA3-512</td><td>=</td><td>"+hashString+"</td></tr></table>");
            this->repaint(); // 立即刷新

        }else{

        }


        if(fileB1.exists()){


            long frequencyB3 = 0;
            //long frequencyB2 = 0;

            if (!fileB1.open(QIODevice::ReadOnly)) {
                qWarning() << "无法打开文件：" << pathB;
                //return false;
            }

            QCryptographicHash fileB3Hash(QCryptographicHash::Sha3_512);

            // 按块读取文件并更新哈希值
            while (!fileB1.atEnd()) {
                QByteArray data = fileB1.read(bufferSize); // 每次读取 bufferSize KB
                fileB3Hash.addData(data);

                double scheduleDouble = 1.0*frequencyB3*bufferSize/fileB1.size()*100;
                int scheduleInt = qFloor(scheduleDouble);
                if(scheduleInt>100){
                    scheduleInt = 100;
                }

                ui->infoLabel->setText("正在计算文件B哈希值..."+QString::number(scheduleDouble, 'f', 2)+"%"); // 保留两位小数;
                ui->progressBar->setValue(scheduleInt);
                frequencyB3++;
            }

            // 关闭文件
            fileB1.close();

            // 哈希值的十六进制表示
            QString hashString = fileB3Hash.result().toHex();

            ui->infoLabel->setText("正在计算文件B哈希值...100%"); // 保留两位小数;
            ui->progressBar->setValue(100);

            ui->outputTextBrowser->append("<table><tr><td>文件B SHA3-512</td><td>=</td><td>"+hashString+"</td></tr></table>");

            this->repaint(); // 立即刷新


        }

        //QThread::msleep(3000); // 毫秒级

        ui->infoLabel->setText("完成！");

        this->repaint(); // 立即刷新


    }




    QTimer::singleShot(50, [this]() {

        ui->startComparingButton->setText("开始对比");
        ui->startComparingButton->setStyleSheet("background-color: #ffffff;");
        ui->resetButton->setStyleSheet("background-color: #ffffff;");
        ui->pathALineEdit->setStyleSheet("background-color: #ffffff;");
        ui->pathBLineEdit->setStyleSheet("background-color: #ffffff;");

        ui->startComparingButton->setEnabled(true);
        ui->resetButton->setEnabled(true);
        ui->pathALineEdit->setEnabled(true);
        ui->pathBLineEdit->setEnabled(true);



        //qDebug() << "工作完成！";

    });

    ui->outputTextBrowser->append("代码运行时间:"+QString::number(timer.elapsed())+"ms");
    //qDebug() << "代码运行时间（毫秒）:" << timer.elapsed();
    //ui->startComparingButton->setEnabled(true);
}




void MainWindow::on_pathALineEdit_textEdited(const QString &arg1)
{
    ui->infoLabel->setText("文件A路径改变");
    ui->infoLabel->setStyleSheet("color:red;");

}


void MainWindow::on_pathBLineEdit_textEdited(const QString &arg1)
{
    ui->infoLabel->setText("文件B路径改变");
    ui->infoLabel->setStyleSheet("color:red;");
}



void MainWindow::on_resetButton_clicked()
{
    ui->pathALineEdit->clear();
    ui->pathBLineEdit->clear();
    ui->infoLabel->clear();
    ui->infoLabel->setStyleSheet("color:#000000;");

    ui->progressBar->setValue(0);
    ui->outputTextBrowser->clear();
    numberOfComparisons=0;
}





void MainWindow::on_aboutButton_clicked()
{
    // 如果尚未创建新窗体，进行实例化
    if (!aboutWindow) {
        aboutWindow = new AboutWindow(this); // 设置主窗体为父对象
    }
    aboutWindow->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);// 设置仅保留关闭按钮
    aboutWindow->show(); // 显示新窗体
    aboutWindow->raise(); // 将窗体置于最前
    aboutWindow->activateWindow(); // 激活窗口
}

