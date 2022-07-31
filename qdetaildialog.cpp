#include "qdetaildialog.h"
#include "ui_qdetaildialog.h"

QDetailDialog::QDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDetailDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Detail Information"));
    setWindowIcon(QIcon(":/icons/icons/qt-logo.svg"));

    shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor("#dcdcdc");
    shadow->setBlurRadius(20);
    shadow->setOffset(5);

    ui->frame->setGraphicsEffect(shadow);
}

QDetailDialog::~QDetailDialog()
{
    delete ui;
}

void QDetailDialog::setFileName(const QString &file_name)
{
    ui->currentFile->setText(file_name);
}

void QDetailDialog::setMode(const QString &analysis_mode)
{
    ui->mode->setText(analysis_mode);
}

void QDetailDialog::setLengthSet(const PktParse::LengthSet &length)
{
    ui->linkLength->setText(QString::number(length.l_addr));
    ui->typeLength->setText(QString::number(length.type));
    ui->caLength->setText(QString::number(length.c_addr));
    ui->cotLength->setText(QString::number(length.cot));
    ui->oaLength->setText(QString::number(length.o_addr));
    ui->qualiferLength->setText(QString::number(length.qualifier));
    ui->describerLength->setText(QString::number(length.describer));
    ui->snLength->setText(QString::number(length.sn));
}
