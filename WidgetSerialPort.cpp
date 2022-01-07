/**
* @file WidgetSerialPort.cpp
* @author Anil Kumar
* @date 7Jan2022
* @brief This is WidgetSerialPort QWidget class.
* It is handling QSerialPort with data flow (Open/close, read/write and errors).
*/

#include "WidgetSerialPort.h"
#include "ui_WidgetSerialPort.h"

/**
 * @brief WidgetSerialPort::WidgetSerialPort
 * @param parent
 * @param TabName
 */
WidgetSerialPort::WidgetSerialPort(QWidget *parent, QString TabName) :
    QWidget(parent),
    ui(new Ui::WidgetSerialPort)
{
    ui->setupUi(this);

    strTabName = TabName;
    SerialPort_SEND = new QSerialPort;
    connect(SerialPort_SEND,SIGNAL(readyRead()),this,SLOT(sl_ReadData()));
    connect(SerialPort_SEND, SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(sl_ReadOutputError(QSerialPort::SerialPortError)));


    ui->comboBox_PortList->setEditable(true);
    ui->comboBox_PortList->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_PortList->findChild<QLineEdit*>()->setStyleSheet("QLineEdit "
                                                                  "{background-color: rgb(119,136,153); color: rgb(240, 255, 255);}");
    ui->lineEdit->installEventFilter(this);

    ui->comboBox_BaudRate->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_DataBits->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_FlowControl->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_OpenMode->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_Parity->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_StopBits->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    timerHealth = new QTimer;
    connect(timerHealth,SIGNAL(timeout()),this, SLOT(sl_Connect()));

    timerSerialPortList = new QTimer;
    connect(timerSerialPortList,SIGNAL(timeout()),this, SLOT(sl_UpdateSerialPortList()));

    timerSerialPortList->start(10000);
}

WidgetSerialPort::~WidgetSerialPort()
{
    delete ui;
}
/**
 * @brief WidgetSerialPort::m_GetTabName
 * @return Tab name
 */
QString WidgetSerialPort::m_GetTabName()
{
    return strTabName;
}

/**
 * @brief WidgetSerialPort::eventFilter
 * @param watched
 * @param event
 * @return
 */
bool WidgetSerialPort::eventFilter(QObject *obj, QEvent *event)
{
//    if(ui->lineEdit->hasFocus())
//    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        int keyInt = keyEvent->key();

////        keyEvent->modifiers();
////        if(keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_D)
////            qDebug()<<"Press Ctrl + D";
//        if((keyInt== Qt::Key_Enter || keyInt == Qt::Key_Return)&& ui->lineEdit->text().size()>0)
//        {
//            qDebug()<<"Pressed" <<QKeySequence(keyInt).toString(QKeySequence::NativeText)
//                   <<keyEvent->text();
//            return true;
//        }
//    }

//    return QObject::eventFilter(watched, event);



    QKeyEvent *KE = static_cast<QKeyEvent*>(event);
    if (KE->key() == Qt::Key_Return || KE->key() == Qt::Key_Enter)
    {
        //qDebug() << "eventFilter" << obj;
        if (obj == ui->lineEdit)
        {
            qDebug()<<"Pressed" <<QKeySequence(KE->key()).toString(QKeySequence::NativeText)
                   <<KE->text();
        }
        else
            return QObject::eventFilter(obj, event);

        return true;  //return TRUE, stopping event propagation
    }
        return QObject::eventFilter(obj, event);
}

/**
 * @brief WidgetSerialPort::m_Get_uiDataBits
 * This method get Data Bits as per selected comboBox_DataBits
 * @return
 */
QSerialPort::DataBits WidgetSerialPort::m_Get_uiDataBits()
{
    QSerialPort::DataBits dBits;

    switch (ui->comboBox_DataBits->currentIndex())
    {
    case 0:
        dBits = QSerialPort::Data5;
        break;
    case 1:
        dBits = QSerialPort::Data6;
        break;
    case 2:
        dBits = QSerialPort::Data7;
        break;
    case 3:
        dBits = QSerialPort::Data8;
        break;
    default:
        dBits = QSerialPort::Data8;
        break;
    }

    return dBits;
}

/**
 * @brief WidgetSerialPort::m_Get_uiParity
 * This method get Parity as per selected comboBox_Parity
 * @return
 */
QSerialPort::Parity WidgetSerialPort::m_Get_uiParity()
{
    QSerialPort::Parity parity;

    switch (ui->comboBox_Parity->currentIndex())
    {
    case 0:
        parity = QSerialPort::NoParity;
        break;
    case 1:
        parity = QSerialPort::EvenParity;
        break;
    case 2:
        parity = QSerialPort::OddParity;
        break;
    case 3:
        parity = QSerialPort::SpaceParity;
        break;
    case 4:
        parity = QSerialPort::MarkParity;
        break;
    default:
        parity = QSerialPort::NoParity;
        break;
    }

    return parity;
}

/**
 * @brief WidgetSerialPort::m_Get_uiStopBits
 * This method get Stop Bits as per selected comboBox_StopBits
 * @return
 */
QSerialPort::StopBits WidgetSerialPort::m_Get_uiStopBits()
{
    QSerialPort::StopBits sBits;

    switch (ui->comboBox_StopBits->currentIndex())
    {
    case 0:
        sBits = QSerialPort::OneStop;
        break;
    case 1:
        sBits = QSerialPort::TwoStop;
        break;
    case 2:
        sBits = QSerialPort::OneAndHalfStop;
        break;
    default:
        sBits = QSerialPort::OneStop;
        break;
    }

    return sBits;
}

/**
 * @brief WidgetSerialPort::m_Get_uiFlowControl
 * This method get Flow control as per selected comboBox_FlowControl
 * @return
 */
QSerialPort::FlowControl WidgetSerialPort::m_Get_uiFlowControl()
{
    QSerialPort::FlowControl flowCtrl;

    switch (ui->comboBox_FlowControl->currentIndex())
    {
    case 0:
        flowCtrl = QSerialPort::NoFlowControl;
        break;
    case 1:
        flowCtrl = QSerialPort::HardwareControl;
        break;
    case 2:
        flowCtrl = QSerialPort::SoftwareControl;
        break;
    default:
        flowCtrl = QSerialPort::NoFlowControl;
        break;
    }

    return flowCtrl;
}

/**
 * @brief WidgetSerialPort::m_Get_uiOpenMode
 * This method get Open Mode as per selected comboBox_OpenMode
 * @return
 */
QSerialPort::OpenModeFlag WidgetSerialPort::m_Get_uiOpenMode()
{
    QSerialPort::OpenModeFlag OpenMode;

    switch (ui->comboBox_OpenMode->currentIndex())
    {
    case 0:
        OpenMode = QSerialPort::ReadOnly;
        break;
    case 1:
        OpenMode = QSerialPort::WriteOnly;
        break;
    case 2:
        OpenMode = QSerialPort::ReadWrite;
        break;
    default:
        OpenMode = QSerialPort::ReadWrite;
        break;
    }

    return OpenMode;
}

/**
 * @brief WidgetSerialPort::on_pushButton_Send_clicked
 * This is slot of Send button
 */
void WidgetSerialPort::on_pushButton_Send_clicked()
{
    if(SerialPort_SEND->write(ui->lineEdit->text().toLocal8Bit())!=-1)
    {
        if(!ui->checkBox_Hex->isChecked())
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                          "<font color=\"blue\"> SEND : "+ui->lineEdit->text()+"</font>");
        else
        {
            QByteArray tmp; tmp.append(ui->lineEdit->text());
            QByteArray hex = tmp.toHex(' ');
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                          "<font color=\"blue\"> SEND : "+QString::fromStdString(hex.toStdString())+"</font>");
        }
    }
    else
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"NOT write"<<SerialPort_SEND->errorString();

}


/**
 * @brief WidgetSerialPort::sl_UpdateSerialPortList
 * This slot fetch system plugged serial port on every 10sec by timerSerialPortList
 * and update port name in combo box of ui.
 */
void WidgetSerialPort::sl_UpdateSerialPortList()
{
    if(isSerialPortOpen)
        return;

    QList<QSerialPortInfo> List = QSerialPortInfo::availablePorts();

    if(sPortList.size() != List.size())
    {
        sPortList.clear();
        sPortList = List;
        ui->comboBox_PortList->clear();
        foreach (QSerialPortInfo portInfo , sPortList)
            ui->comboBox_PortList->addItem("/dev/"+portInfo.portName());
    }
}

/**
 * @brief WidgetSerialPort::sl_ReadData
 * This is slot to read RX data when data is available.
 */
void WidgetSerialPort::sl_ReadData()
{
    QByteArray data = SerialPort_SEND->readAll();
    if(data.size()>0)
    {
        if(!ui->checkBox_Hex->isChecked())
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                               "<font color=\"green\"> RECV : "+QString::fromStdString(data.toStdString())+"</font>");
        else
        {
            QByteArray hex = data.toHex(' ');
//            QByteArray hex = data.toHex();  // ICNEXUS
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                          "<font color=\"green\"> RECV : "+QString::fromStdString(hex.toStdString())+"</font>");
        }
    }
}

/**
 * @brief WidgetSerialPort::on_pushButton_Open_clicked
 * This is slot of Open button
 */
void WidgetSerialPort::on_pushButton_Open_clicked()
{
    strPortName     = ui->comboBox_PortList->currentText();
    iBaudrate       = ui->comboBox_BaudRate->currentText().toInt();
    iDataBits       = m_Get_uiDataBits();
    iParity         = m_Get_uiParity();
    iStopBits       = m_Get_uiStopBits();
    iFlowControl    = m_Get_uiFlowControl();
    iOpenMode       = m_Get_uiOpenMode();

    if(!isSerialPortOpen)
    {
        sl_Connect();
        timerHealth->start(10000);
    }
    else
    {
        timerHealth->stop();
        SerialPort_SEND->close();
        ui->pushButton_Open->setText("Open");
        isSerialPortOpen = false;
        ui->label_Status->setText(strPortName+" Closed at "+QString::number(iBaudrate));
        emit si_ChangeStatus(this,PORT_STATE::CLOSE);
    }

//    if(!SerialPort_SEND->isOpen())
//        QMessageBox::warning(this,"Serial port",strPortName+" is closed. !!\n"+SerialPort_SEND->errorString());
}


/**
 * @brief WidgetSerialPort::sl_Connect
 * This is slot of timerHealth, to check and reconnect at 10 seconds interval
 */
void WidgetSerialPort::sl_Connect()
{
    if(!SerialPort_SEND->isOpen())
    {
        SerialPort_SEND->setPortName(strPortName);

        if(SerialPort_SEND->open(iOpenMode))
        {
            SerialPort_SEND->setBaudRate(iBaudrate);
            SerialPort_SEND->setDataBits(iDataBits);
            SerialPort_SEND->setParity(iParity);
            SerialPort_SEND->setStopBits(iStopBits);
            SerialPort_SEND->setFlowControl(iFlowControl);

            qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"Successfully Open Serial port"<<SerialPort_SEND->portName();
            isSerialPortOpen = true;
            ui->label_Status->setText(strPortName+" Opened at "+QString::number(iBaudrate));
            strTabName = "Serial ("+SerialPort_SEND->portName()+")";
            ui->pushButton_Open->setText("Close");
            emit si_ChangeStatus(this,PORT_STATE::OPEN);
        }
        else
        {
            qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<SerialPort_SEND->portName()<<"Not open "<<SerialPort_SEND->errorString();
            isSerialPortOpen = false;
            ui->label_Status->setText(strPortName+" Closed at "+QString::number(iBaudrate));
            emit si_ChangeStatus(this,PORT_STATE::CLOSE);
        }
    }
}

/**
 * @brief WidgetSerialPort::sl_ReadOutputError
 * This slot handle Serial port error
 * @param e type of serial port error
 */
void WidgetSerialPort::sl_ReadOutputError(QSerialPort::SerialPortError e)
{
    QString error = SerialPort_SEND->portName()+" ";

    if(e != QSerialPort::NoError)
    {
        if(SerialPort_SEND->isOpen())
            SerialPort_SEND->close();

        ui->label_Status->setText(strPortName+" Closed at "+QString::number(iBaudrate));
        isSerialPortOpen = false;
        ui->pushButton_Open->setText("Open");
        emit si_ChangeStatus(this,PORT_STATE::CLOSE);
    }

    QString ErrorMsg;
    switch (e)
    {
    case QSerialPort::NoError:
        ErrorMsg = error+"No error occurred";
        //qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    case QSerialPort::DeviceNotFoundError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"An error occurred while attempting to open an non-existing device.";
        ErrorMsg = error + "Serial port not found !!";
        break;
    case QSerialPort::PermissionError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"An error occurred while attempting to open an already opened device by another process"
                  " or a user not having enough permission and credentials to open."
                  "An error occurred while attempting to open an already opened device in this object.";
        ErrorMsg = error + "Permission denied !!";
        break;
    case QSerialPort::OpenError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"An error occurred while attempting to open an already opened device in this object.";
        ErrorMsg = error +"Serial port is already open";
        break;
    case QSerialPort::ParityError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"Parity error detected by the hardware while reading data. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+ "Parity error detected by the hardware while reading data.!!";
        break;
    case QSerialPort::FramingError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"Framing error detected by the hardware while reading data. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+"Framing error detected by the hardware while reading data.!!";
        break;
    case QSerialPort::BreakConditionError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"Break condition detected by the hardware on the input line. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+"Break condition detected by the hardware on the input line.!!";
        break;
    case QSerialPort::WriteError:
        ErrorMsg = error+"An I/O error occurred while writing the data.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    case QSerialPort::ReadError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<error+"An I/O error occurred while reading the data.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    case QSerialPort::ResourceError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"An I/O error occurred when a resource becomes unavailable,"
                  " e.g. when the device is unexpectedly removed from the system.";
        ErrorMsg = error+"An I/O error occurred when a resource becomes unavailable.!!";
        break;
    case QSerialPort::UnsupportedOperationError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"The requested device operation is not supported "
                  "or prohibited by the running operating system.";
        ErrorMsg = error+"The requested device operation is not supported.!! ";
        break;
    case QSerialPort::UnknownError:
        ErrorMsg = error+"An unidentified error occurred.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    case QSerialPort::TimeoutError:
        ErrorMsg = error+"A timeout error occurred.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    case QSerialPort::NotOpenError:
        ErrorMsg = error+"This error occurs when an operation is executed that can only be successfully performed if the device is open.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<ErrorMsg;
        break;
    default:
        break;
    }

//    if(e != QSerialPort::NoError)
//    {
//        QMessageBox::warning(this,"Serial port Error",ErrorMsg);
//        timerHealth->stop();
//    }
}

/**
 * @brief WidgetSerialPort::on_pushButton_Clear_clicked
 * this is slot to clear plainTextEdit area.
 */
void WidgetSerialPort::on_pushButton_Clear_clicked()
{
    ui->plainTextEdit->clear();
}

/**
 * @brief SerialCOM::on_pushButton_Save_clicked
 * This is slot to save all log data into a file.
 */
void WidgetSerialPort::on_pushButton_Save_clicked()
{
    if(ui->plainTextEdit->toPlainText().size()<=0)
    {
        QMessageBox::warning(this,"Copy","No data available. !!");
        return;
    }

    const QStringList filters({"text (*.txt)","log (*.log)"});
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilters(filters);
    fileDialog.exec();

    QString fnameName = fileDialog.selectedFiles().first();

    qDebug()<<fileDialog.selectedNameFilter() <<fileDialog.selectedMimeTypeFilter();
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << fnameName;
    QFile file(fnameName);
    file.open(QIODevice::WriteOnly);
    QTextStream wrt(&file);
    wrt<<ui->plainTextEdit->toPlainText()<<endl;
    file.close();
}
