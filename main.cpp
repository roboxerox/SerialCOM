/**
* @file main.cpp
* @author Anil Kumar
* @date 15May2021
* @brief This is main class.
*/
#include "SerialCOM.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QTextCodec>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QCoreApplication app(argc, argv);
    app.setApplicationName(STR_PRODUCTNAME);
    app.setApplicationVersion(STR_SOFTWARE_VERSION);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.setApplicationDescription(STR_FILEDESCRIPTION);
    parser.process(app);

    SerialCOM w;
    w.show();

    return app.exec();
}
