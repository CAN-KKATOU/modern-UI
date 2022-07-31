#ifndef QTITLEBAR_H
#define QTITLEBAR_H

#include <QWidget>
#include <QIcon>
#include <QMouseEvent>
#include <QPointF>

namespace Ui {
class QTitleBar;
}

class QTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit QTitleBar(QWidget *parent = nullptr);
    ~QTitleBar();

    void setTitle(const QString&);

    void setMaxFlag(bool);

    void setMainWindow(QWidget*);

private slots:
    void on_closeBtn_clicked();

    void on_minBtn_clicked();

    void on_maxBtn_clicked();

private:
    Ui::QTitleBar *ui;

    void mouseDoubleClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    bool windowMaxFlag = false;

    bool m_isPressed = false;

    QPointF m_startMovePos;

    QWidget *mainWindow;

signals:
    void signalButtonMinClicked();

    void signalButtonMaxClicked();

    void signalButtonCloseClicked();
};

#endif // QTITLEBAR_H
