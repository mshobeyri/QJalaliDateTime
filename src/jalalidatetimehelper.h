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

QVector<int> gregorianToJalali(int gy, int gm, int gd) {
    QVector<int> gdm = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int jy = gy <= 1600 ? 0 : 979;

    gy -= (gy <= 1600) ? 621 : 1600;
    int gy2  = (gm > 2) ? (gy + 1) : gy;
    int days = (365 * gy) + (int((gy2 + 3) / 4)) - (int((gy2 + 99) / 100)) +
        (int((gy2 + 399) / 400)) - 80 + gd + gdm[gm - 1];
    jy += 33 * (int(days / 12053));
    days %= 12053;
    jy += 4 * (int(days / 1461));
    days %= 1461;
    jy += int((days - 1) / 365);
    if (days > 365)
        days = (days - 1) % 365;
    int jm = (days < 186) ? 1 + int(days / 31) : 7 + int((days - 186) / 30);
    int jd = 1 + ((days < 186) ? (days % 31) : ((days - 186) % 30));
    return {jy, jm, jd};
}
QVector<int> jalaliToGregorian(int jy, int jm, int jd) {
    int gy=(jy<=979)?621:1600;
    jy-=(jy<=979)?0:979;
    int days=(365*jy) +((int(jy/33))*8) +(int(((jy%33)+3)/4))
        +78 +jd +((jm<7)?(jm-1)*31:((jm-7)*30)+186);
    gy+=400*(int(days/146097));
    days%=146097;
    if(days > 36524){
        gy+=100*(int(--days/36524));
        days%=36524;
        if(days >= 365)days++;
    }
    gy+=4*(int((days)/1461));
    days%=1461;
    gy+=int((days-1)/365);
    if(days > 365)days=(days-1)%365;
    int gd=days+1;
    QVector<int> sal_a={0,31,((gy%4==0 && gy%100!=0) || (gy%400==0))?29:28,31,30,31,30,31,31,30,31,30,31};
    int gm;
    for(gm=0;gm<13;gm++){
        int v=sal_a[gm];
        if(gd <= v)break;
        gd-=v;
    }
    return {gy,gm,gd};
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
