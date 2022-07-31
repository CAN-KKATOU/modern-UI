#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTimer>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include "src/packet_parsing/BasePacket.h"
#include "src/packet_parsing/PacketFactory.h"
#include "jsonthread.h"
#include "openthread.h"
#include "qjsonmodel.h"
#include "customproxymodel.h"
#include "highlighter.h"
#include "qdetaildialog.h"
#include "jsonparser.h"
#include "positionfinder.h"

namespace Ui {
class QPacketForm;
}

using namespace PktParse;

extern QReadWriteLock rwLock;

class QPacketForm : public QWidget
{
    Q_OBJECT

public:
    explicit QPacketForm(QWidget *parent = nullptr);
    ~QPacketForm() override;

    bool setFileName(const QString& file_name);

    void setModeAndLength(const QString& mode, const LengthSet& length);

    [[nodiscard]] QString getFileName() const { return fileName; }

    std::shared_ptr<QStringList> getOutputBuffer(bool withTime, bool withSend, bool withPosition);

signals:

    void parseJson(const QJsonObject&);

    void setPositionJson(const QJsonObject&);

    void parseJsonStr(const std::string&);

    void setPositionJsonStr(const std::string&);

    void findPosition(QList<QString>);

public slots:

    void setFilter();

    void onOpenFinished();

private:
    Ui::QPacketForm *ui;

    QString modeStr;
    QString fileName;
    LengthSet length;
    AnalysisMode analysisMode;

    int index;

    QStringList *buffer;

    QStandardItemModel *tableModel;
    QItemSelectionModel *theSelection;
    QJsonModel *treeModel;
    CustomProxyModel *proxyModel;

    OpenThread *openThread;

    QThread *jsonThread;
    QThread *positionThread;

    QTimer *refreshTimer;

    Highlighter *highlighter;

    QDetailDialog *dialog;

    QGraphicsDropShadowEffect *shadow;
};

#endif // MESSAGEFORM_H
