#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>

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
    QString pathA = ui->pathALineEdit->text();
    QString pathB = ui->pathBLineEdit->text();

    //pathA = R"(C:\工作区\Workspace\test.txt)";
    //pathB = R"(C:\工作区\Workspace\test2.txt)";

    //ui->pathALineEdit->setText(pathA);
    QFile file(pathA);

    if (!file.exists()) {
        qDebug() << "文件不存在:" << pathA;
    }else {
        qDebug() << "找到文件:" << pathA;
    }

}

