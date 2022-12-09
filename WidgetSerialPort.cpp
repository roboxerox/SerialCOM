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
WidgetSerialPort::WidgetSerialPort(QWidget *parent, int Tab_num) :
    QWidget(parent),
    ui(new Ui::WidgetSerialPort)
{
    ui->setupUi(this);

    TabNum = Tab_num;
    strTabName = "Serial "+ QString::number(Tab_num);
    SerialPort_RxTx = new QSerialPort;
    connect(SerialPort_RxTx,SIGNAL(readyRead()),this,SLOT(sl_ReadData()));
    connect(SerialPort_RxTx, SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(sl_ReadOutputError(QSerialPort::SerialPortError)));


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

    timerSerialPortList->start(5000);
}

WidgetSerialPort::~WidgetSerialPort()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    if(SerialPort_RxTx->isOpen())
    {
        SerialPort_RxTx->flush();
        SerialPort_RxTx->close();
    }
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
    Q_UNUSED(obj)
    if(ui->lineEdit->hasFocus() && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *KE = static_cast<QKeyEvent*>(event);
        if (KE->key() == Qt::Key_Return || KE->key() == Qt::Key_Enter)
        {
            qDebug()<<"Pressed" <<QKeySequence(KE->key()).toString(QKeySequence::NativeText);
            if(ui->lineEdit->text().size()>0)
            {
                on_pushButton_Send_clicked();
                return true;
            }
        }
    }
    return false ;
}

/**
 * @brief WidgetSerialPort::m_Get_uiDataBits
 * This method get Data Bits as per selected comboBox_DataBits
 * @return
 */
QSerialPort::DataBits WidgetSerialPort::m_Get_uiDataBits()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    switch (ui->comboBox_DataBits->currentIndex())
    {
    case 0:
        return QSerialPort::Data5;
    case 1:
        return QSerialPort::Data6;
    case 2:
        return QSerialPort::Data7;
    case 3:
        return QSerialPort::Data8;
    default:
        return QSerialPort::Data8;
    }

    return QSerialPort::Data8;
}

/**
 * @brief WidgetSerialPort::m_Get_uiParity
 * This method get Parity as per selected comboBox_Parity
 * @return
 */
QSerialPort::Parity WidgetSerialPort::m_Get_uiParity()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    switch (ui->comboBox_Parity->currentIndex())
    {
    case 0:
        return QSerialPort::NoParity;
    case 1:
        return QSerialPort::EvenParity;
    case 2:
        return QSerialPort::OddParity;
    case 3:
        return QSerialPort::SpaceParity;
    case 4:
        return QSerialPort::MarkParity;
    default:
        return QSerialPort::NoParity;
    }

    return QSerialPort::NoParity;
}

/**
 * @brief WidgetSerialPort::m_Get_uiStopBits
 * This method get Stop Bits as per selected comboBox_StopBits
 * @return
 */
QSerialPort::StopBits WidgetSerialPort::m_Get_uiStopBits()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    switch (ui->comboBox_StopBits->currentIndex())
    {
    case 0:
        return QSerialPort::OneStop;
    case 1:
        return QSerialPort::TwoStop;
    case 2:
        return QSerialPort::OneAndHalfStop;
    default:
        return QSerialPort::OneStop;
    }

    return QSerialPort::OneStop;
}

/**
 * @brief WidgetSerialPort::m_Get_uiFlowControl
 * This method get Flow control as per selected comboBox_FlowControl
 * @return
 */
QSerialPort::FlowControl WidgetSerialPort::m_Get_uiFlowControl()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    switch (ui->comboBox_FlowControl->currentIndex())
    {
    case 0:
        return QSerialPort::NoFlowControl;
    case 1:
        return QSerialPort::HardwareControl;
    case 2:
        return QSerialPort::SoftwareControl;
    default:
        return QSerialPort::NoFlowControl;
    }

    return QSerialPort::NoFlowControl;
}

/**
 * @brief WidgetSerialPort::m_Get_uiOpenMode
 * This method get Open Mode as per selected comboBox_OpenMode
 * @return
 */
QSerialPort::OpenModeFlag WidgetSerialPort::m_Get_uiOpenMode()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;
    switch (ui->comboBox_OpenMode->currentIndex())
    {
    case 0:
        return QSerialPort::ReadOnly;
    case 1:
        return QSerialPort::WriteOnly;
    case 2:
        return QSerialPort::ReadWrite;
    default:
        return QSerialPort::ReadWrite;
    }

    return QSerialPort::ReadWrite;
}

/**
 * @brief WidgetSerialPort::on_pushButton_Send_clicked
 * This is slot of Send button
 */
void WidgetSerialPort::on_pushButton_Send_clicked()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

    if(SerialPort_RxTx->write(ui->lineEdit->text().toLocal8Bit())!=-1)
    {
        if(!ui->checkBox_Hex->isChecked())
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                          "<font color=\"blue\"> SEND : "+ui->lineEdit->text()+"</font>");
        else
        {
            QByteArray tmp; tmp.append(ui->lineEdit->text());
            QByteArray hex = tmp.toHex();//tmp.toHex(' ');
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                          "<font color=\"blue\"> SEND : "+QString::fromStdString(hex.toStdString())+"</font>");
        }
    }
    else
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<<"NOT write"<<SerialPort_RxTx->errorString();

}


/**
 * @brief WidgetSerialPort::sl_UpdateSerialPortList
 * This slot fetch system plugged serial port on every 10sec by timerSerialPortList
 * and update port name in combo box of ui.
 */
void WidgetSerialPort::sl_UpdateSerialPortList()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

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
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

    QByteArray data = SerialPort_RxTx->readAll();
    if(data.size()>0)
    {
        if(!ui->checkBox_Hex->isChecked())
            ui->plainTextEdit->appendHtml(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+
                                               "<font color=\"green\"> RECV : "+QString::fromStdString(data.toStdString())+"</font>");
        else
        {
//            QByteArray hex = data.toHex(' ');
            QByteArray hex = data.toHex();  // ICNEXUS
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
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

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
        SerialPort_RxTx->close();
        ui->pushButton_Open->setText("Open");
        isSerialPortOpen = false;
        ui->label_Status->setText(strPortName+" closed at "+QString::number(iBaudrate));
        strTabName = "Serial "+ QString::number(TabNum);
        emit si_ChangeStatus(this,PORT_STATE::CLOSE);
    }
}


/**
 * @brief WidgetSerialPort::sl_Connect
 * This is slot of timerHealth, to check and reconnect at 10 seconds interval
 */
void WidgetSerialPort::sl_Connect()
{
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

    if(!SerialPort_RxTx->isOpen())
    {
        SerialPort_RxTx->setPortName(strPortName);

        if(SerialPort_RxTx->open(iOpenMode))
        {
            SerialPort_RxTx->setBaudRate(iBaudrate);
            SerialPort_RxTx->setDataBits(iDataBits);
            SerialPort_RxTx->setParity(iParity);
            SerialPort_RxTx->setStopBits(iStopBits);
            SerialPort_RxTx->setFlowControl(iFlowControl);

            qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
                   <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"Successfully Open Serial port"<<SerialPort_RxTx->portName();
            isSerialPortOpen = true;
            ui->label_Status->setText(strPortName+" opened at "+QString::number(iBaudrate));
            strTabName = "Serial ("+SerialPort_RxTx->portName()+")";
            ui->pushButton_Open->setText("Close");
            emit si_ChangeStatus(this,PORT_STATE::OPEN);
        }
        else
        {
            qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
                   <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<SerialPort_RxTx->portName()<<"Not open "<<SerialPort_RxTx->errorString();
            isSerialPortOpen = false;
            ui->label_Status->setText(strPortName+" closed at "+QString::number(iBaudrate));
            strTabName = "Serial "+ QString::number(TabNum);
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
    QString error = SerialPort_RxTx->portName()+" ";

    if(e != QSerialPort::NoError)
    {
        if(SerialPort_RxTx->isOpen())
            SerialPort_RxTx->close();

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
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"An error occurred while attempting to open an non-existing device.";
        ErrorMsg = error + "Serial port not found !!";
        break;
    case QSerialPort::PermissionError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"An error occurred while attempting to open an already opened device by another process"
                  " or a user not having enough permission and credentials to open."
                  "An error occurred while attempting to open an already opened device in this object.";
        ErrorMsg = error + "Permission denied !!";
        break;
    case QSerialPort::OpenError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"An error occurred while attempting to open an already opened device in this object.";
        ErrorMsg = error +"Serial port is already open";
        break;
    case QSerialPort::ParityError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"Parity error detected by the hardware while reading data. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+ "Parity error detected by the hardware while reading data.!!";
        break;
    case QSerialPort::FramingError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"Framing error detected by the hardware while reading data. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+"Framing error detected by the hardware while reading data.!!";
        break;
    case QSerialPort::BreakConditionError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"Break condition detected by the hardware on the input line. This value is obsolete."
                  " We strongly advise against using it in new code.";
        ErrorMsg = error+"Break condition detected by the hardware on the input line.!!";
        break;
    case QSerialPort::WriteError:
        ErrorMsg = error+"An I/O error occurred while writing the data.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<ErrorMsg;
        break;
    case QSerialPort::ReadError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<error+"An I/O error occurred while reading the data.";
        break;
    case QSerialPort::ResourceError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"An I/O error occurred when a resource becomes unavailable,"
                  " e.g. when the device is unexpectedly removed from the system.";
        ErrorMsg = error+"An I/O error occurred when a resource becomes unavailable.!!";
        break;
    case QSerialPort::UnsupportedOperationError:
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"The requested device operation is not supported "
                  "or prohibited by the running operating system.";
        ErrorMsg = error+"The requested device operation is not supported.!! ";
        break;
    case QSerialPort::UnknownError:
        ErrorMsg = error+"An unidentified error occurred.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<ErrorMsg;
        break;
    case QSerialPort::TimeoutError:
        ErrorMsg = error+"A timeout error occurred.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<ErrorMsg;
        break;
    case QSerialPort::NotOpenError:
        ErrorMsg = error+"This error occurs when an operation is executed that can only be successfully performed if the device is open.";
        qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")<< "File :"<<__FILE__
               <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<ErrorMsg;
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
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << "File :"<<__FILE__
           <<"Line :"<<__LINE__<<"Func :"<<__FUNCTION__ <<"strTabName -"<< strTabName;

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

    QString fnameName;
    if(fileDialog.selectedFiles().count())
        fnameName = fileDialog.selectedFiles().first();
    else
        return;

    if(fileDialog.selectedNameFilter().contains(".txt"))
        fnameName.append(".txt");
    else
        fnameName.append(".log");
    qDebug()<<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") << fnameName;

    QFile file(fnameName);
    file.open(QIODevice::WriteOnly);
    QTextStream wrt(&file);
    wrt<<ui->plainTextEdit->toPlainText()<<endl;
    file.close();
}
