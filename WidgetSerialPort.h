/**
* @file WidgetSerialPort.h
* @author Anil Kumar
* @date 7Jan2022
* @brief This is header of WidgetSerialPort class.
*/
#ifndef WIDGETSERIALPORT_H
#define WIDGETSERIALPORT_H

#include <QWidget>
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>


namespace PORT_STATE {
enum
{
    UNKNOWN = -1,
    CLOSE = 0,
    OPEN
};
}

namespace Ui {
class WidgetSerialPort;
}

class WidgetSerialPort : public QWidget
{
    Q_OBJECT
    QSerialPort::DataBits m_Get_uiDataBits();
    QSerialPort::Parity m_Get_uiParity();
    QSerialPort::StopBits m_Get_uiStopBits();
    QSerialPort::FlowControl m_Get_uiFlowControl();
    QSerialPort::OpenModeFlag m_Get_uiOpenMode();
public:
    explicit WidgetSerialPort(QWidget *parent = 0,QString TabName ="Serial0");
    ~WidgetSerialPort();
    QSerialPort * SerialPort_SEND;/**< Object of QSerialPort **/
    QTimer *timerHealth;/**< Object of QTimer **/
    QTimer *timerSerialPortList;/**< Object of QTimer **/
    QString strPortName;/**< QString Object for Serial Port Name **/
    qint32 iBaudrate = QSerialPort::Baud9600; /**< qint32 Object for Serial Port BaudRate **/
    QSerialPort::DataBits iDataBits = QSerialPort::Data8; /**< Object for Serial Port Data Bits **/
    QSerialPort::Parity iParity = QSerialPort::NoParity; /**< Object for Serial Port Parity **/
    QSerialPort::StopBits iStopBits = QSerialPort::OneStop; /**< Object for Serial Port Stop Bits **/
    QSerialPort::FlowControl iFlowControl = QSerialPort::NoFlowControl; /**< Object for Serial Port Flow Control **/
    QSerialPort::OpenModeFlag iOpenMode = QSerialPort::ReadWrite; /**< Object for Serial Port Open Mode **/
    bool isSerialPortOpen = false; /**< boolean Object for serial port open or not **/
    QString m_GetTabName();
private slots:
    void sl_UpdateSerialPortList();
    void sl_ReadData();
    void on_pushButton_Open_clicked();
    void sl_Connect();
    void sl_ReadOutputError(QSerialPort::SerialPortError e);
    void on_pushButton_Send_clicked();
    void on_pushButton_Clear_clicked();
    void on_pushButton_Save_clicked();
protected:
    virtual bool eventFilter(QObject * watched, QEvent * event);
signals:
    void si_ChangeStatus(WidgetSerialPort*,int portState);
private:
    Ui::WidgetSerialPort *ui;
    QList<QSerialPortInfo> sPortList;
    QString strTabName;
};

#endif // WIDGETSERIALPORT_H
