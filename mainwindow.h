#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aboutwindow.h" // 引入新窗体的头文件

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    long numberOfComparisons = 0;



private slots:
    void on_startComparingButton_clicked();

    void on_resetButton_clicked();

    void on_pathBLineEdit_textEdited(const QString &arg1);

    void on_pathALineEdit_textEdited(const QString &arg1);

    void on_aboutButton_clicked();

private:
    Ui::MainWindow *ui;
    AboutWindow *aboutWindow; // 新窗体的指针

};
#endif // MAINWINDOW_H
