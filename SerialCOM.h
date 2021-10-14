/**
* @file SerialCOM.h
* @author Anil Kumar
* @date 15May2021
* @brief This is header of SerialCOM class.
*/
#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>

#define VER_MAJOR               QString("0")
#define VER_MINOR               QString("1")
#define VER_BUILD               QString("2a")
#define STR_PRODUCTNAME         QString("SerialCOM")
#define STR_SOFTWARE_VERSION    QString(VER_MAJOR+"."+VER_MINOR+"."+VER_BUILD)
#define STR_FILEDESCRIPTION     QString("Serial port communication")

namespace Ui {
class SerialCOM;
}

class SerialCOM : public QMainWindow
{
    Q_OBJECT
    QSerialPort::DataBits m_Get_uiDataBits();
    QSerialPort::Parity m_Get_uiParity();
    QSerialPort::StopBits m_Get_uiStopBits();
    QSerialPort::FlowControl m_Get_uiFlowControl();
    QSerialPort::OpenModeFlag m_Get_uiOpenMode();
public:
    explicit SerialCOM(QWidget *parent = 0);
    ~SerialCOM();
    QSerialPort * SerialPort_SEND;/**< Object of QSerialPort **/
    QTimer *timerHealth;/**< Object of QTimer **/
    QString strPortName;/**< QString Object for Serial Port Name **/
    qint32 iBaudrate = QSerialPort::Baud9600; /**< qint32 Object for Serial Port BaudRate **/
    QSerialPort::DataBits iDataBits = QSerialPort::Data8; /**< Object for Serial Port Data Bits **/
    QSerialPort::Parity iParity = QSerialPort::NoParity; /**< Object for Serial Port Parity **/
    QSerialPort::StopBits iStopBits = QSerialPort::OneStop; /**< Object for Serial Port Stop Bits **/
    QSerialPort::FlowControl iFlowControl = QSerialPort::NoFlowControl; /**< Object for Serial Port Flow Control **/
    QSerialPort::OpenModeFlag iOpenMode = QSerialPort::ReadWrite; /**< Object for Serial Port Open Mode **/
    bool isSerialPortOpen = false; /**< boolean Object for serial port open or not **/
public slots:
private slots:
    void sl_ReadData();
    void on_pushButton_Open_clicked();
    void sl_Connect();
    void sl_ReadOutputError(QSerialPort::SerialPortError e);
    void on_pushButton_Send_clicked();
    void on_pushButton_Clear_clicked();
    void sl_AboutUs(bool ab);
    void on_pushButton_Save_clicked();
    void sl_Close(bool cl);
protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);
private:
    Ui::SerialCOM *ui;
};

#endif // SERIALCOM_H
