#ifndef JALALIDATETIMEHELPER_H
#define JALALIDATETIMEHELPER_H
#include <QDebug>
#include <QLocale>
#include <QString>

QString
formatNumber(const QString& text, bool arabicFormat) {
    if (!arabicFormat)
        return text;
    QString formated = text;
    formated.replace("1", "۱")
        .replace("2", "۲")
        .replace("3", "۳")
        .replace("4", "۴")
        .replace("5", "۵")
        .replace("6", "۶")
        .replace("7", "۷")
        .replace("8", "۸")
        .replace("9", "۹")
        .replace("0", "۰");
    return formated;
}

QString
pad(int number, int padding, bool arabicFormat) {
    auto s = QString::number(number);
    while (s.length() < padding) {
        s.prepend("0");
    }
    return formatNumber(s, arabicFormat);
}
QString
repeat(const QChar& c, int length) {
    QString result;
    while (result.length() < length) {
        result.append(c);
    }
    return result;
}

static QString gz{"ق.ظ"};
static QString bz{"ب.ظ"};

QString
nameOfMonth(int month, bool shortFormat) {
    QString result;
    switch (month) {
    case 1:
        result = "فروردین";
        break;
    case 2:
        result = "اردیبهشت";
        break;
    case 3:
        result = "خرداد";
        break;
    case 4:
        result = "تیر";
        break;
    case 5:
        result = "مرداد";
        break;
    case 6:
        result = "شهریور";
        break;
    case 7:
        result = "مهر";
        break;
    case 8:
        result = "آبان";
        break;
    case 9:
        result = "آذر";
        break;
    case 10:
        result = "دی";
        break;
    case 11:
        result = "بهمن";
        break;
    case 12:
        result = "اسفند";
        break;
    }
    if (shortFormat)
        return result.left(3);
    else {
        return result;
    }
}
QString
nameOfDay(int day, bool shortFormat) {
    QString result;
    return QLocale{QLocale::Persian}.dayName(
        day, shortFormat ? QLocale::ShortFormat : QLocale::LongFormat);
}


#endif // JALALIDATETIMEHELPER_H
