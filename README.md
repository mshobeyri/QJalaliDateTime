# QJalaliDateTime

fast qt base c++ library support jalali date time support qml too.


## build
you can build this lib along your project. clone the project file in your project folder.

```bash
git clone https://github.com/mshobeyri/QJalaliDateTime.git
````
in your project .pro file include ``src/QJalaliDateTime.pri`` from this project path.

```bash
include(./QJalaliDateTime/src/QJalaliDateTime.pri)
```
now your project build this library too.

## usage

### qt example

``` c++
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
}

// output:
// QJalaliDateTime(1398/7/20 12:55:32 +0330)
// "۱۳۹۸/۰۷/۲۰ ۱۲:۵۵"
// "98 07 20"
```

#### qml example
 first register this lib for qml in `main.cpp` using `QJalaliDateTime::registerQml(engine);`
 
 ``` cpp
 
#include "qjalalidatetime.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int
main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    
    QJalaliDateTime::registerQml(engine); // <<- regitster using this line
    
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
```
and then you can use it in your project as context property `JDateTime` and registered object 'JalaliDateTime' by import `QtQuick.JalaliDateTime 1.0` like this:

```js
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.JalaliDateTime 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Jalali Date Time")
    JalaliDateTime{
        id: idt1
        gyear: 1990
        gmonth: 12
        gday: 8
    }
    JalaliDateTime{
        id: idt2
        jyear: 1369
        jmonth: 9
        jday: 17
    }

    Component.onCompleted: {
        // use context property
        console.log(JDateTime.jyear,JDateTime.jmonth,JDateTime.jday)
        console.log(JDateTime.gyear,JDateTime.gmonth,JDateTime.gday)

        // use registerd Component
        console.log(idt1.jyear,idt1.jmonth,idt1.jday)
        console.log(idt2.gyear,idt2.gmonth,idt2.gday)

        // change registered Component
        idt1.gyear = 1991
        console.log(idt1.jyear,idt1.jmonth,idt1.jday)

    }
}

// output:
// qml: 1398 7 20
// qml: 2019 10 12
// qml: 1369 9 17
// qml: 1990 12 8
// qml: 1370 9 17
```
