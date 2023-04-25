/**
* @file SerialCOM.cpp
* @author Anil Kumar
* @date 15May2021
* @brief This is SerialCOM main UI class.
* It is handling WidgetSerialPort widget.
*/
#include "SerialCOM.h"
#include "ui_SerialCOM.h"

/**
 * @brief SerialCOM::SerialCOM
 * @param parent
 */
SerialCOM::SerialCOM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialCOM)
{
    ui->setupUi(this);

    connect(ui->actionAbout_Us,SIGNAL(triggered(bool)),this,SLOT(sl_AboutUs(bool)));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(sl_Close(bool)));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(sl_OpenNew(bool)));

    connect(ui->tabWidget->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(sl_CloseTab(int)));

    sl_OpenNew(true);
}

/**
 * @brief SerialCOM::~SerialCOM
 */
SerialCOM::~SerialCOM()
{
    delete ui;
}

/**
 * @brief SerialCOM::sl_AboutUs
 * This is slot to gives details of app.
 * @param ab
 */
void SerialCOM::sl_AboutUs(bool ab)
{
    qDebug().noquote()<<DEBUG_PREFIX(__FILE__,__LINE__,__FUNCTION__);
    Q_UNUSED(ab)
    QMessageBox *cAbout = new QMessageBox(this);
    cAbout->setIconPixmap(QPixmap(":images/wing_com.png").scaled(QSize(60,40)));
    cAbout->setStyleSheet("QMessageBox {"
                         "color: rgb(255, 255, 255);"
                         "background-color: rgb(215, 214, 255);"
                         "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(112,128,144, 255), stop:1 rgba(255, 255, 255, 255));"
                     "}");
    cAbout->setText("SerialCOM\nVersion "+STR_SOFTWARE_VERSION+"\n\n"
                    "The program is provided AS IS with NO WARRANTY OF ANY KIND.");
    cAbout->setStandardButtons(QMessageBox::Close);
    cAbout->exec();
}

/**
 * @brief SerialCOM::sl_Close
 * @param cl
 */
void SerialCOM::sl_Close(bool cl)
{
    qDebug().noquote()<<DEBUG_PREFIX(__FILE__,__LINE__,__FUNCTION__);
    Q_UNUSED(cl)
    close();
}

/**
 * @brief SerialCOM::sl_OpenNew
 * @param nStat
 * This slot add new tab widget of WidgetSerialPort
 */
void SerialCOM::sl_OpenNew(bool nStat)
{
    qDebug().noquote()<<DEBUG_PREFIX(__FILE__,__LINE__,__FUNCTION__);
    Q_UNUSED(nStat)

    int num = 0;
    for(int i = 0;i<ui->tabWidget->count();i++)
    {
        if(!sp_widgets.value(i))
        {
            num = i;
            break;
        }
    }

    if(num==0)
        num = ui->tabWidget->count();

    WidgetSerialPort *uiSerial = new WidgetSerialPort(this, num);
    connect(uiSerial,SIGNAL(si_ChangeStatus(WidgetSerialPort*,int)),this,SLOT(sl_ChangeTabStatus(WidgetSerialPort*,int)));
    ui->tabWidget->addTab(uiSerial, QIcon(":/images/disconnected.png"),"Serial "+QString::number(num));

    sp_widgets.insert(num,uiSerial);
}

void SerialCOM::sl_CloseTab(int index)
{
    qDebug().noquote()<<DEBUG_PREFIX(__FILE__,__LINE__,__FUNCTION__);
    if(index>0)
    {
        WidgetSerialPort *uiSerial  =  qobject_cast<WidgetSerialPort*>(ui->tabWidget->widget(index));
        sp_widgets.remove(uiSerial->TabNum);
        uiSerial->deleteLater();
        ui->tabWidget->removeTab(index);
    }
}

/**
 * @brief SerialCOM::sl_ChangeTabStatus
 * @param uiSerialPort
 * @param state
 */
void SerialCOM::sl_ChangeTabStatus(WidgetSerialPort *uiSerialPort, int state)
{
    qDebug().noquote()<<DEBUG_PREFIX(__FILE__,__LINE__,__FUNCTION__);
    switch (state) {
    case PORT_STATE::OPEN:
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(uiSerialPort),QIcon(":/images/connected.png"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(uiSerialPort),uiSerialPort->m_GetTabName());
        break;
    case PORT_STATE::CLOSE:
        ui->tabWidget->setTabIcon(ui->tabWidget->indexOf(uiSerialPort),QIcon(":/images/disconnected.png"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(uiSerialPort),uiSerialPort->m_GetTabName());
        break;
    default:
        break;
    }
}
