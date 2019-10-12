#include "qjalalidatetime.h"
#include <QCoreApplication>
#include <QtDebug>

int
main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    QJalaliDateTime t;
    qDebug() << t;
    qDebug() << t.toString();
    qDebug() << t.toString("yy MM dd",QJalaliDateTime::LatinNumbers);
    return a.exec();
}
