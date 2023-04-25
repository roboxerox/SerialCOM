/**
* @file SerialCOM.h
* @author Anil Kumar
* @date 15May2021
* @brief This is header of SerialCOM class.
*/
#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QMainWindow>
#include <WidgetSerialPort.h>


namespace Ui {
class SerialCOM;
}

class SerialCOM : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialCOM(QWidget *parent = 0);
    ~SerialCOM();

public slots:
private slots:
    void sl_AboutUs(bool ab);
    void sl_Close(bool cl);
    void sl_OpenNew(bool nStat);
    void sl_CloseTab(int index);
    void sl_ChangeTabStatus(WidgetSerialPort* uiSerialPort,int state);
private:
    Ui::SerialCOM *ui;
    QMap<int,bool> index_stat;
    QMap<int,WidgetSerialPort*> sp_widgets;
};

#endif // SERIALCOM_H
