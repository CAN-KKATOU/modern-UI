#include "qtitlebar.h"
#include "ui_qtitlebar.h"

QTitleBar::QTitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QTitleBar)
{
    ui->setupUi(this);
}

QTitleBar::~QTitleBar()
{
    delete ui;
}

void QTitleBar::setTitle(const QString &title)
{
    ui->label->setText(title);
}

void QTitleBar::setMaxFlag(bool flag)
{
    windowMaxFlag = flag;
}

void QTitleBar::setMainWindow(QWidget *window)
{
    mainWindow = window;
}

void QTitleBar::on_closeBtn_clicked()
{
    emit signalButtonCloseClicked();
}


void QTitleBar::on_minBtn_clicked()
{
    emit signalButtonMinClicked();
}


void QTitleBar::on_maxBtn_clicked()
{
    emit signalButtonMaxClicked();

    if (windowMaxFlag)
        ui->maxBtn->setIcon(QIcon(":/icons/icons/original_svg/window_undock.svg"));
    else
        ui->maxBtn->setIcon(QIcon(":/icons/icons/original_svg/square.svg"));
}

void QTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signalButtonMaxClicked();

    if (windowMaxFlag)
        ui->maxBtn->setIcon(QIcon(":/icons/icons/original_svg/window_undock.svg"));
    else
        ui->maxBtn->setIcon(QIcon(":/icons/icons/original_svg/square.svg"));

    return QWidget::mouseDoubleClickEvent(event);
}

void QTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (windowMaxFlag)
        m_isPressed = false;
    else {
        m_isPressed = true;
        m_startMovePos = event->globalPosition();
    }

    return QWidget::mousePressEvent(event);
}

void QTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPointF movePoint = event->globalPosition() - m_startMovePos;

        QPointF widgetPos = mainWindow->pos();

        m_startMovePos = event->globalPosition();

        mainWindow->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }

    return QWidget::mouseMoveEvent(event);
}

void QTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;

    return QWidget::mouseReleaseEvent(event);
}

