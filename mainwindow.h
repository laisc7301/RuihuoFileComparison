#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_pushButton_clicked();

    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
