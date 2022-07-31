#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include <QGraphicsDropShadowEffect>
#include "src/packet_parsing/BasePacket.h"

namespace Ui {
class QDetailDialog;
}

class QDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QDetailDialog(QWidget *parent = nullptr);
    ~QDetailDialog() override;

    void setFileName(const QString& file_name);

    void setMode(const QString& analysis_mode);

    void setLengthSet(const PktParse::LengthSet& length_set);

private:
    Ui::QDetailDialog *ui;

    QGraphicsDropShadowEffect *shadow;
};

#endif // DETAILDIALOG_H
