#ifndef DRAGLINEEDIT_H
#define DRAGLINEEDIT_H

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class DragLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit DragLineEdit(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override; // 拖拽进入事件
    void dropEvent(QDropEvent *event) override;           // 放置事件
};

#endif // DRAGLINEEDIT_H
