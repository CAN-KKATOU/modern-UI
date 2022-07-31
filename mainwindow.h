#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include "outputthread.h"
#include "QPacketForm.h"
#include "src/packet_parsing/BasePacket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_menuBtn_clicked();

    void on_menuCloseBtn_clicked();

    void on_setBtn_clicked();

    void on_infoBtn_clicked();

    void on_helpBtn_clicked();

    void on_homeBtn_clicked();

    void on_fileOpenBtn_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_messageOpenBtn_clicked();

    void on_cancelBtn_clicked();

    void on_pathBtn_clicked();

    void on_outputBtn_clicked();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void on_outputBtn_2_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_linkLength_valueChanged(int arg1);

    void on_typeLength_valueChanged(int arg1);

    void on_caLength_valueChanged(int arg1);

    void on_cotLength_valueChanged(int arg1);

    void on_oaLength_valueChanged(int arg1);

    void on_qualiferLength_valueChanged(int arg1);

    void on_describerLength_valueChanged(int arg1);

    void on_snLength_valueChanged(int arg1);

    void openClicked();

    void onOutputFinished();

private:
    Ui::MainWindow *ui;

    int sildeBarWidth;
    int menuWidth;

    QPropertyAnimation *sildeBarAnim;
    bool isSildeBarExpand = true;

    QPropertyAnimation *menuAnim;
    bool isMenuShow = false;

    PktParse::LengthSet length;

    QString mode;

    QStringList modeList;

    QString fileName;

    QGraphicsDropShadowEffect *shadow_1;
    QGraphicsDropShadowEffect *shadow_2;
    QGraphicsDropShadowEffect *shadow_3;
    QGraphicsDropShadowEffect *shadow_4;
    QGraphicsDropShadowEffect *shadow_5;
    QGraphicsDropShadowEffect *shadow_6;

    QPacketForm *currentForm;
    outputThread *output_thread;
};
#endif // MAINWINDOW_H
