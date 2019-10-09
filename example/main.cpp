#include "qjalalidatetime.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QTimeZone>
#include <QtDebug>

int
main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    qDebug() << QJalaliDateTime{QDateTime::currentDateTime()};

    qDebug() << QDateTime::currentDateTime();
    return a.exec();
}
