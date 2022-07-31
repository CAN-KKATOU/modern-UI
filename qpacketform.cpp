#include "qpacketform.h"
#include "ui_qpacketform.h"

QPacketForm::QPacketForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QPacketForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);

    // moreBtn阴影效果设置
    shadow = new QGraphicsDropShadowEffect(this);
    shadow->setColor("#696969");
    shadow->setBlurRadius(20);
    shadow->setOffset(3);

    ui->moreBtn->setGraphicsEffect(shadow);

    // 表格模型初始化
    tableModel = new QStandardItemModel(0, 4, this);

    tableModel->setHeaderData(0, Qt::Horizontal, "时间");
    tableModel->setHeaderData(1, Qt::Horizontal, "接收/发送");
    tableModel->setHeaderData(2, Qt::Horizontal, "报文类型");

    // 代理模型初始化
    proxyModel = new CustomProxyModel(this);
    proxyModel->setSourceModel(tableModel);
    theSelection = new QItemSelectionModel(proxyModel);
    proxyModel->setTimeColumn(0);
    proxyModel->setSendColumn(1);
    proxyModel->setTypeColumn(2);

    // 高亮器初始化
    highlighter = new Highlighter(ui->textBrowser->document());

    // 树模型初始化
    treeModel = new QJsonModel(this);

    // 写入缓存初始化
    buffer = new QStringList();

    // 多个线程初始化
    openThread = new OpenThread(this);


    jsonThread = new QThread(this);

    JsonParser *jParser = new JsonParser;
    jParser->setModel(treeModel);
    jParser->moveToThread(jsonThread);

    positionThread = new QThread(this);

    PositionFinder *pFinder = new PositionFinder;
    pFinder->moveToThread(positionThread);

    // 界面视图和模型绑定
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionModel(theSelection);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->viewport()->setMouseTracking(true);
    ui->treeView->setModel(treeModel);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->sendCombo->addItem("全部");
    ui->sendCombo->addItem("发送");
    ui->sendCombo->addItem("接收");

    dialog = new QDetailDialog(this);

    connect(ui->tableView, &QTableView::clicked, [=](const QModelIndex &index){
        if (index.isValid())
        {
            if (proxyModel->data(proxyModel->index(index.row(), 2)).toString() != "未知报文")
            {
                QString data = proxyModel->data(proxyModel->index(index.row(), 3)).toString();

                highlighter->setStartAndEnd(0, 0);
                ui->textBrowser->setText(data);

                auto pktTemp = PacketFactory::getSPtr(data.toStdString(), analysisMode, length);
                emit parseJsonStr(pktTemp->toString());
                emit setPositionJsonStr(pktTemp->getPositionStr());
            }
        }
    });

    connect(ui->treeView, &QTreeView::clicked, [=](const QModelIndex& index){
        QModelIndex curIndex = index;

        QList<QString> keys;
        while (curIndex.isValid())
        {
            keys.push_front(treeModel->data(treeModel->index(curIndex.row(), 0, curIndex.parent()), Qt::DisplayRole).toString());
            curIndex = curIndex.parent();
        }

        emit findPosition(keys);
    });

    connect(ui->moreBtn, &QPushButton::clicked, [=](bool checked){
        dialog->exec();
    });

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timerId, [=](){
        QString time;
        QString isSend;
        QString data;

        while(!buffer->isEmpty())
        {
            QReadLocker locker(&rwLock);
            QString line = buffer->takeFirst();

            time = line.mid(0, 23);
            isSend = line.mid(24, 2);
            data = line.mid(27);

            tableModel->insertRow(tableModel->rowCount());
            tableModel->setData(tableModel->index(index, 0), time);
            tableModel->setData(tableModel->index(index, 1), isSend);
            tableModel->setData(tableModel->index(index, 3), data);
            tableModel->setData(tableModel->index(index, 2), QString::fromStdString(PacketFactory::parse_type(data.toStdString(), analysisMode, length)));

            index++ ;
        }

        // 打开本地文件时调用
        if (buffer->isEmpty())
            refreshTimer->stop();
    });

    connect(openThread, SIGNAL(finished()), this, SLOT(onOpenFinished()));

    connect(ui->fromDateTime, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime& dateTime){
        proxyModel->setMaxDateTime(ui->toDateTime->dateTime());
    });
    connect(ui->toDateTime, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime& dateTime){
        proxyModel->setMaxDateTime(dateTime);
    });


    connect(ui->typeCombo, &QComboBox::currentTextChanged, this, &QPacketForm::setFilter);
    connect(ui->sendCombo, &QComboBox::currentTextChanged, this, &QPacketForm::setFilter);

    connect(jsonThread, &QThread::finished, jParser, &JsonParser::deleteLater);
    connect(this, &QPacketForm::parseJson, jParser, &JsonParser::parse);
    connect(this, &QPacketForm::parseJsonStr, jParser, &JsonParser::parseStr);
    jsonThread->start();

    connect(positionThread, &QThread::finished, pFinder, &PositionFinder::deleteLater);
    connect(this, &QPacketForm::setPositionJson, pFinder, &PositionFinder::getPositionJson);
    connect(this, &QPacketForm::setPositionJsonStr, pFinder, &PositionFinder::getPositionJsonStr);
    connect(this, &QPacketForm::findPosition, pFinder, &PositionFinder::find);
    connect(pFinder, &PositionFinder::positionFound, [=](int startIndex, int endIndex){
        highlighter->setStartAndEnd(startIndex, endIndex);
        highlighter->rehighlight();
    });
    positionThread->start();

    // 默认过滤为半年之前至今
    ui->fromDateTime->setDateTime(QDateTime::currentDateTime().addDays(-180));
    ui->toDateTime->setDateTime(QDateTime::currentDateTime());

    index = 0;
}

QPacketForm::~QPacketForm()
{
    if (openThread)
    {
        if (openThread->isRunning())
        {
            openThread->quit();
            openThread->wait();
        }
    }

    if (jsonThread->isRunning())
    {
        jsonThread->quit();
        jsonThread->wait();
    }

    if (positionThread->isRunning())
    {
        positionThread->quit();
        positionThread->wait();
    }

    buffer->clear();
    delete buffer;

    delete ui;
    qDebug() << "messageForm: " + fileName + " has been released.";
}

bool QPacketForm::setFileName(const QString &file_name)
{
    fileName = file_name;

    if (!openThread->setFileName(fileName))
        return false;

    openThread->setBuffer(buffer);
    openThread->start();

    QFileInfo info(fileName);
    setWindowTitle(info.fileName());

    dialog->setFileName(fileName);

    return true;
}

void QPacketForm::setModeAndLength(const QString &mode, const LengthSet& length)
{
    this->length = length;
    ui->typeCombo->addItem("全部");
    if (mode == "默认101协议" or mode == "自定义101协议")
    {
        modeStr = "101协议";
        analysisMode = AnalysisMode::DEFAULT_101;
    }
    else if(mode == "默认104协议" or mode == "自定义104协议")
    {
        modeStr = "104协议";
        analysisMode = AnalysisMode::DEFAULT_104;
    }
    else if (mode == "默认佛山101协议" or mode == "自定义佛山101协议")
    {
        modeStr = "佛山101协议";
        analysisMode = AnalysisMode::SOUTH_101;
    }
    else if(mode == "默认佛山104协议" or mode == "自定义佛山104协议")
    {
        modeStr = "佛山104协议";
        analysisMode = AnalysisMode::SOUTH_104;
    }

    for (const auto& val : PacketFactory::get_types_list(analysisMode))
    {
        ui->typeCombo->addItem(QString::fromStdString(val));
    }

    setFilter();
    qDebug() << tr("Analysis mode has been set.");

    refreshTimer->start(500);

    dialog->setMode(modeStr);
    dialog->setLengthSet(length);
}

void QPacketForm::setFilter()
{
    QString pattern_1;
    QString pattern_2;

    if (ui->typeCombo->currentText() == "全部")
    {
        for (const auto& val : PacketFactory::get_types_list(analysisMode))
        {
            pattern_1 += QString::fromStdString(val) + "|";
        }

    }
    else pattern_1 = ui->typeCombo->currentText() + "|";

    if (ui->sendCombo->currentText() == "全部")
        pattern_2 = "接收|发送";
    else pattern_2 = ui->sendCombo->currentText();

    // qDebug() << pattern_1 + pattern_2;

    QRegularExpression re(pattern_1 + pattern_2, QRegularExpression::NoPatternOption | QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(re);

    QJsonObject nullObject;
    nullObject.insert("","");

    treeModel->loadJsonObject(nullObject);

    ui->textBrowser->clear();
}

void QPacketForm::onOpenFinished()
{
    disconnect(openThread, SIGNAL(finished()), this, SLOT(onOpenFinished()));

    delete openThread;

    openThread = nullptr;
}

std::shared_ptr<QStringList> QPacketForm::getOutputBuffer(bool withTime, bool withSend, bool withPosition) {
    qDebug() << "Get output buffer.";

    auto output_buffer = std::make_shared<QStringList>();

    for (int index = 0; index < tableModel->rowCount(); index++)
    {
        QString line;

        QString time = tableModel->data(tableModel->index(index, 0)).toString();
        if (withTime)
            line += time + " ";

        qDebug() << "output time" + time;

        QString send = tableModel->data(tableModel->index(index, 1)).toString();
        if (withSend)
            line += send + " ";

        bool isSend = send == "发送";
        QString origin_data = tableModel->data(tableModel->index(index, 3)).toString();

        auto pktTemp = PacketFactory::getSPtr(origin_data.toStdString(), analysisMode, length);
        line += QString::fromStdString(pktTemp->toString(true)) + "\n";
        if (withPosition)
            line += QString::fromStdString(pktTemp->getPositionStr(true)) + "\n";

        output_buffer->append(line);
    }

    return output_buffer;
}

