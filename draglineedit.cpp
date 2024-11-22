#include "draglineedit.h"
#include <QDebug>

DragLineEdit::DragLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setAcceptDrops(true); // 启用拖拽功能
}

// 拖拽进入事件
void DragLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) { // 判断是否为文件 URL
        event->acceptProposedAction(); // 接受拖拽
    } else {
        event->ignore();
    }
}

// 放置事件
void DragLineEdit::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls(); // 获取文件 URL 列表
        if (!urls.isEmpty()) {
            QString filePath = urls.first().toLocalFile(); // 获取第一个文件路径
            setText(filePath); // 将路径填入 QLineEdit
            emit textEdited(filePath); // 手动触发 textEdited 信号
            //qDebug() << "Dropped file path:" << filePath;

        }
    }
}
