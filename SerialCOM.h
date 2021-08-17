#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>

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
    QSerialPort * SerialPort_SEND;
    QTimer *timerHealth;
    QString strPortName;
    qint32 iBaudrate = QSerialPort::Baud9600;
    QSerialPort::DataBits iDataBits = QSerialPort::Data8;
    QSerialPort::Parity iParity = QSerialPort::NoParity;
    QSerialPort::StopBits iStopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl iFlowControl = QSerialPort::NoFlowControl;
    QSerialPort::OpenModeFlag iOpenMode = QSerialPort::ReadWrite;
    bool isSerialPortOpen = false;
public slots:
private slots:
    void sl_ReadData();
    void on_pushButton_Open_clicked();
    void sl_Connect();
    void sl_ReadOutputError(QSerialPort::SerialPortError e);
    void on_pushButton_Send_clicked();
    void on_pushButton_Clear_clicked();
    void sl_AboutUs(bool ab);
private:
    Ui::SerialCOM *ui;
};

#endif // SERIALCOM_H
