#include "qjalalidatetime.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QTimeZone>
#include <QtDebug>

int
main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    qDebug() << QDateTime::fromString("111","dM");
    return a.exec();
}
