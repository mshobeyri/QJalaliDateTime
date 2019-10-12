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
