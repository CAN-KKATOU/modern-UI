#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Message Analysis"));
    //setWindowIcon(QIcon(":/icons/icons/qt-logo.svg"));

    // mdiArea初始化
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabsClosable(true);
    ui->mdiArea->cascadeSubWindows();

    // 设置侧边栏和二级菜单宽度
    sildeBarWidth = 200;
    menuWidth = 300;

    // 初始化隐藏二级菜单
    ui->centerMenuContainer->setMaximumWidth(0);

    // 侧边栏动画效果初始化
    sildeBarAnim = new QPropertyAnimation(this);
    sildeBarAnim->setTargetObject(ui->leftMenuContainer);
    sildeBarAnim->setPropertyName("maximumWidth");
    sildeBarAnim->setEasingCurve(QEasingCurve::InOutQuad);
    sildeBarAnim->setStartValue(sildeBarWidth);
    sildeBarAnim->setEndValue(40);
    sildeBarAnim->setDuration(300);

    // 二级菜单动画效果初始化
    menuAnim = new QPropertyAnimation(this);
    menuAnim->setTargetObject(ui->centerMenuContainer);
    menuAnim->setPropertyName("maximumWidth");
    menuAnim->setEasingCurve(QEasingCurve::InOutQuad);
    menuAnim->setStartValue(0);
    menuAnim->setEndValue(menuWidth);
    menuAnim->setDuration(300);

    // 连接侧栏和HomePage打开按钮槽函数
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(openClicked()));
    connect(ui->openBtn_2, SIGNAL(clicked()), this, SLOT(openClicked()));

    // openPage初始化
    /***************************************************************************/

    // 默认打开报文设置
    modeList.append("默认101协议");
    modeList.append("自定义101协议");
    modeList.append("默认104协议");
    modeList.append("自定义104协议");
    modeList.append("默认佛山101协议");
    modeList.append("自定义佛山101协议");
    modeList.append("默认佛山104协议");
    modeList.append("自定义佛山104协议");
    mode = "默认101协议";
    length = {true, 2, 1, 2, 2, 2, 1, 1, 2};

    ui->comboBox->addItems(modeList);
    ui->comboBox->setCurrentIndex(0);

    // 设置文件路径后才能使用打开按钮
    ui->messageOpenBtn->setDisabled(true);

    // 阴影效果设置
    shadow_1 = new QGraphicsDropShadowEffect(this);
    shadow_2 = new QGraphicsDropShadowEffect(this);
    shadow_3 = new QGraphicsDropShadowEffect(this);

    shadow_1->setColor("#dcdcdc");
    shadow_1->setBlurRadius(20);
    shadow_1->setOffset(5);

    shadow_2->setColor("#dcdcdc");
    shadow_2->setBlurRadius(20);
    shadow_2->setOffset(5);

    shadow_3->setColor("#dcdcdc");
    shadow_3->setBlurRadius(20);
    shadow_3->setOffset(5);

    ui->frame_6->setGraphicsEffect(shadow_1);
    ui->frame_7->setGraphicsEffect(shadow_2);
    ui->frame_8->setGraphicsEffect(shadow_3);

    // outputPage 初始化
    /***************************************************************************/

    // 导出按钮在设置路径后可用
    ui->outputBtn_2->setDisabled(true);

    // 默认导出带时间、带接收/发送、不带位置
    ui->withTimeCheck->setChecked(true);
    ui->withSendCheck->setChecked(true);
    ui->withPositionCheck->setChecked(false);

    // 阴影效果设置
    shadow_4 = new QGraphicsDropShadowEffect(this);
    shadow_5 = new QGraphicsDropShadowEffect(this);
    shadow_6 = new QGraphicsDropShadowEffect(this);

    shadow_4->setColor("#dcdcdc");
    shadow_4->setBlurRadius(20);
    shadow_4->setOffset(5);

    shadow_5->setColor("#dcdcdc");
    shadow_5->setBlurRadius(20);
    shadow_5->setOffset(5);

    shadow_6->setColor("#dcdcdc");
    shadow_6->setBlurRadius(20);
    shadow_6->setOffset(5);

    ui->frame_10->setGraphicsEffect(shadow_4);
    ui->frame_11->setGraphicsEffect(shadow_5);
    ui->frame_12->setGraphicsEffect(shadow_6);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * 点开或关闭侧边栏
 */
void MainWindow::on_menuBtn_clicked()
{
    if (isSildeBarExpand)
    {
        sildeBarAnim->setStartValue(200);
        sildeBarAnim->setEndValue(50);
        isSildeBarExpand = false;
        sildeBarAnim->start();
    } else
    {
        sildeBarAnim->setStartValue(50);
        sildeBarAnim->setEndValue(200);
        isSildeBarExpand = true;
        sildeBarAnim->start();
    }
}


/**
 * 关闭二级菜单
 */
void MainWindow::on_menuCloseBtn_clicked()
{
    if (isMenuShow)
    {
        menuAnim->setStartValue(menuWidth);
        menuAnim->setEndValue(0);
        isMenuShow = false;
        menuAnim->start();
    }
}


/**
 * 打开或切换设置二级菜单
 */
void MainWindow::on_setBtn_clicked()
{
    if (!isMenuShow)
    {
        menuAnim->setStartValue(0);
        menuAnim->setEndValue(300);
        isMenuShow = true;
        menuAnim->start();
    }

    ui->stackedWidget->setCurrentWidget(ui->setPage);
}


/**
 * 打开或切换信息二级菜单
 */
void MainWindow::on_infoBtn_clicked()
{
    if (!isMenuShow)
    {
        menuAnim->setStartValue(0);
        menuAnim->setEndValue(300);
        isMenuShow = true;
        menuAnim->start();
    }

    ui->stackedWidget->setCurrentWidget(ui->InfoPage);
}


/**
 * 打开或切换帮助二级菜单
 */
void MainWindow::on_helpBtn_clicked()
{
    if (!isMenuShow)
    {
        menuAnim->setStartValue(0);
        menuAnim->setEndValue(300);
        isMenuShow = true;
        menuAnim->start();
    }

    ui->stackedWidget->setCurrentWidget(ui->helpPage);
}


/**
 * 打开主页面
 */
void MainWindow::on_homeBtn_clicked()
{
    if (ui->mdiArea->subWindowList().count() == 0) // mdiArea有打开报文
        ui->mainContentContainer->setCurrentWidget(ui->homePage);
    else // mdiArea没有打开报文
        ui->mainContentContainer->setCurrentWidget(ui->dataPage);

    // 按钮效果设置
    ui->homeBtn->setStyleSheet("background-color: #1f232a;");
    ui->openBtn->setStyleSheet("background-color: transparent;");
    ui->outputBtn->setStyleSheet("background-color: transparent;");
}


/**
 * 打开打开文件页面
 */
void MainWindow::openClicked()
{
    ui->mainContentContainer->setCurrentWidget(ui->openPage);

    ui->openBtn->setStyleSheet("background-color: #1f232a;");
    ui->homeBtn->setStyleSheet("background-color: transparent;");
    ui->outputBtn->setStyleSheet("background-color: transparent;");
}


void MainWindow::on_fileOpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "打开报文文件", "C:\\Users\\", "*.txt");
    ui->lineEdit->setText(fileName);
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QFileInfo file(arg1);
    if (!file.exists())
    {
        ui->tipLabel->setText(tr("File not exists"));
        ui->messageOpenBtn->setDisabled(true);
    }
    else {
        ui->tipLabel->setText("");
        ui->messageOpenBtn->setEnabled(true);
    }
}


/**
 * 选择解析协议
 * @param index
 */
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
    {// 默认101
        ui->linkLength->setValue(2);
        ui->typeLength->setValue(1);
        ui->caLength->setValue(2);
        ui->cotLength->setValue(2);
        ui->oaLength->setValue(2);
        ui->qualiferLength->setValue(1);
        ui->describerLength->setValue(1);
        ui->snLength->setValue(2);

        ui->linkLength->setEnabled(false);
        ui->typeLength->setEnabled(false);
        ui->caLength->setEnabled(false);
        ui->cotLength->setEnabled(false);
        ui->oaLength->setEnabled(false);
        ui->qualiferLength->setEnabled(false);
        ui->describerLength->setEnabled(false);
        ui->snLength->setEnabled(false);
        break;
    } // case 0;
    case 2:
    {// 默认104
        ui->linkLength->setValue(2);
        ui->typeLength->setValue(1);
        ui->caLength->setValue(2);
        ui->cotLength->setValue(2);
        ui->oaLength->setValue(3);
        ui->qualiferLength->setValue(1);
        ui->describerLength->setValue(1);
        ui->snLength->setValue(2);

        ui->linkLength->setEnabled(false);
        ui->typeLength->setEnabled(false);
        ui->caLength->setEnabled(false);
        ui->cotLength->setEnabled(false);
        ui->oaLength->setEnabled(false);
        ui->qualiferLength->setEnabled(false);
        ui->describerLength->setEnabled(false);
        ui->snLength->setEnabled(false);
        break;
    } // case 2;
    case 4:
    {// 佛山101
        ui->linkLength->setValue(2);
        ui->typeLength->setValue(1);
        ui->caLength->setValue(2);
        ui->cotLength->setValue(1);
        ui->oaLength->setValue(2);
        ui->qualiferLength->setValue(1);
        ui->describerLength->setValue(1);
        ui->snLength->setValue(2);

        ui->linkLength->setEnabled(false);
        ui->typeLength->setEnabled(false);
        ui->caLength->setEnabled(false);
        ui->cotLength->setEnabled(false);
        ui->oaLength->setEnabled(false);
        ui->qualiferLength->setEnabled(false);
        ui->describerLength->setEnabled(false);
        ui->snLength->setEnabled(false);
        break;
    } // case 4;
    case 6:
    {// 佛山104
        ui->linkLength->setValue(2);
        ui->typeLength->setValue(1);
        ui->caLength->setValue(2);
        ui->cotLength->setValue(2);
        ui->oaLength->setValue(3);
        ui->qualiferLength->setValue(1);
        ui->describerLength->setValue(1);
        ui->snLength->setValue(2);

        ui->linkLength->setEnabled(false);
        ui->typeLength->setEnabled(false);
        ui->caLength->setEnabled(false);
        ui->cotLength->setEnabled(false);
        ui->oaLength->setEnabled(false);
        ui->qualiferLength->setEnabled(false);
        ui->describerLength->setEnabled(false);
        ui->snLength->setEnabled(false);
        break;
    } // case 6;
    default:
    {// 自定义
        ui->linkLength->setEnabled(true);
        ui->typeLength->setEnabled(true);
        ui->caLength->setEnabled(true);
        ui->cotLength->setEnabled(true);
        ui->oaLength->setEnabled(true);
        ui->qualiferLength->setEnabled(true);
        ui->describerLength->setEnabled(true);
        ui->snLength->setEnabled(true);
        break;
    }
    }
    mode = ui->comboBox->currentText();
}


void MainWindow::on_messageOpenBtn_clicked()
{
    auto form = new QPacketForm(this);
    if (!form->setFileName(fileName))
    { // 报文无法打开
        QMessageBox::information(this, tr("Information"), tr("Could not open ") + fileName + "!", QMessageBox::Ok);
        delete form;
        return;
    }
    form->setModeAndLength(mode, length);
    ui->mdiArea->addSubWindow(form);
    form->show();

    ui->mainContentContainer->setCurrentWidget(ui->dataPage);
    ui->homeBtn->setStyleSheet("background-color: #1f232a;");
    ui->openBtn->setStyleSheet("background-color: transparent;");
    ui->outputBtn->setStyleSheet("background-color: transparent;");
}


void MainWindow::on_cancelBtn_clicked()
{
    if (ui->mdiArea->subWindowList().count() == 0)
        ui->mainContentContainer->setCurrentWidget(ui->homePage);
    else
        ui->mainContentContainer->setCurrentWidget(ui->dataPage);

    ui->homeBtn->setStyleSheet("background-color: #1f232a;");
    ui->openBtn->setStyleSheet("background-color: transparent;");
    ui->outputBtn->setStyleSheet("background-color: transparent;");
}


/**
 * 选择输出路径
 */
void MainWindow::on_pathBtn_clicked()
{

    QString path = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "C:\\Users\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_2->setText(path);
}


/**
 * 切换至导出页面
 */
void MainWindow::on_outputBtn_clicked()
{
    if (ui->mdiArea->subWindowList().count() > 0)
    {
        if (ui->mdiArea->currentSubWindow() == nullptr)
        {
            QMessageBox::information(this, tr("Information"), tr("Programme can not get current Message, please try later!"), QMessageBox::Ok, QMessageBox::No);
        }
        else
        {
            currentForm = static_cast<QPacketForm*>(ui->mdiArea->currentSubWindow()->widget());

            // 显示当前打开报文文件
            ui->lineEdit_3->setText(currentForm->getFileName());
            ui->mainContentContainer->setCurrentWidget(ui->outputPage);

            ui->outputBtn->setStyleSheet("background-color: #1f232a;");
            ui->homeBtn->setStyleSheet("background-color: transparent;");
            ui->openBtn->setStyleSheet("background-color: transparent;");

        }
    } else {
        QMessageBox::information(this, tr("Information"), tr("Program has not found any open files, please open one file and try again."),
                                 QMessageBox::Ok, QMessageBox::No);
    }
}


void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    if (ui->mdiArea->subWindowList().count() == 0)
        ui->mainContentContainer->setCurrentWidget(ui->homePage);
}


void MainWindow::on_outputBtn_2_clicked()
{
   if (ui->mdiArea->subWindowList().count() > 0)
   {
       if (!ui->lineEdit_2->text().isEmpty())
       {
           // 新建导出线程
           output_thread = new outputThread(this);
           output_thread->setFileName(ui->lineEdit_2->text() + "/output.txt");
           output_thread->setBuffer(currentForm->getOutputBuffer(ui->withTimeCheck->isChecked(),
                                                                 ui->withSendCheck->isChecked(),
                                                                 ui->withPositionCheck->isChecked()));
           connect(output_thread, SIGNAL(finished()), this, SLOT(onOutputFinished()));
           output_thread->start();
       }
       else
           QMessageBox::information(this, tr("Information"), tr("Please select the output path!"), QMessageBox::Ok, QMessageBox::No);
   }
}

void MainWindow::onOutputFinished()
{
    QMessageBox::information(this, tr("Information"), tr("Analysis reslut has been output to ") + ui->lineEdit_2->text() + "/output.txt!",
                             QMessageBox::Ok, QMessageBox::No);

    // 断开导出线程的槽函数连接
    disconnect(output_thread, SIGNAL(finished()), this, SLOT(onOutputFinished()));

    // 等待导出线程并释放
    output_thread->wait();
    delete output_thread;
}

/**
 * 检查导出路径是否合法
 * @param arg1
 */
void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    QDir dir(arg1);
    if(dir.exists())
    {
        ui->outputBtn_2->setEnabled(true);
    }
    else
    {
        ui->outputBtn_2->setDisabled(true);
    }
}


void MainWindow::on_linkLength_valueChanged(int arg1)
{
    qDebug() << tr("Link address length set.");

    length.l_addr = arg1;
}


void MainWindow::on_typeLength_valueChanged(int arg1)
{
    qDebug() << tr("Tie indentification length set.");

    length.type = arg1;
}


void MainWindow::on_caLength_valueChanged(int arg1)
{
    qDebug() << tr("Common address length set.");

    length.c_addr= arg1;
}


void MainWindow::on_cotLength_valueChanged(int arg1)
{
    qDebug() << tr("Cause of transmission length set.");

    length.cot = arg1;
}


void MainWindow::on_oaLength_valueChanged(int arg1)
{
    qDebug() << tr("Object Address length set.");

    length.o_addr = arg1;
}


void MainWindow::on_qualiferLength_valueChanged(int arg1)
{
    qDebug() << tr("Qualifer length set.");

    length.qualifier = arg1;
}


void MainWindow::on_describerLength_valueChanged(int arg1)
{
    qDebug() << tr("Describer length set.");

    length.describer = arg1;
}


void MainWindow::on_snLength_valueChanged(int arg1)
{
    qDebug() << tr("SN length set.");

    length.sn = arg1;
}

