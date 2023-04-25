/**
* @file common_header.h
* @author Anil Kumar
* @date 25April2023
* @brief This header has all common includes, Hash defines and enums.
*/
#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>

#define VER_MAJOR               QString("0")
#define VER_MINOR               QString("2")
#define VER_BUILD               QString("1f")
#define STR_PRODUCTNAME         QString("SerialCOM")
#define STR_SOFTWARE_VERSION    QString(VER_MAJOR+"."+VER_MINOR+"."+VER_BUILD)
#define STR_FILEDESCRIPTION     QString("Serial port communication")

#define DEBUG_PREFIX(file,line,func)        QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+" File:"+file+\
                                            " Line:"+QString::number(line)+" Func:"+func+"||"

#endif // COMMON_HEADER_H
