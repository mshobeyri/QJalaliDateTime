#include "qjalalidatetime.h"
#include "jalalidatetimehelper.h"
#include <QDateTime>
#include <QDebug>
#include <QVector>

class QJalaliDateTimePrivate
{
public:
    QDateTime                dateTime;
    QJalaliDateTime::Numbers defaultNumbersView =
        QJalaliDateTime::ArabicIndicNumbers;

    QVector<int> GregorianToJalali(int gy, int gm, int gd) {
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
    QVector<int> GregorianToJalali() {
        auto date = dateTime.date();
        return GregorianToJalali(date.year(), date.month(), date.day());
    }
};
///////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    Message();
    Message(const Message& other);
    ~Message();

    Message(const QString& body, const QStringList& headers);

    QString     body() const;
    QStringList headers() const;

private:
    QString     m_body;
    QStringList m_headers;
};

struct CharHelper {
    CharHelper() {}
    CharHelper(QChar c) : cchar(c) {}
    QChar cchar     = '\0';
    bool  isHotChar = false;
    int   repeat    = 1;
};

struct CharVector : public QVector<CharHelper> {

    bool containAp = false;

    CharVector(const QString& format) {
        bool       scoped = false;
        CharHelper lastChar{'\0'};
        lastChar.isHotChar = true;
        QString fromatTemp;
        if (format.indexOf("a") != -1 || format.indexOf("A") != -1)
            containAp = true;

        for (int i = 0; i < format.length(); i++) {
            const auto& c = format.at(i);
            if (c != lastChar.cchar && lastChar.cchar != '\0') {
                append(lastChar);
                lastChar.cchar = '\0';
            }
            if (c == "'") {
                scoped = !scoped;
                continue;
            }
            if (scoped || QString("aAyMdHhmszt").indexOf(c) == -1) {
                append(CharHelper{c});
                continue;
            }
            if ((c == 'a' && i < format.length() - 1 &&
                 format.at(i + 1) == 'p') ||
                (c == 'A' && i < format.length() - 1 &&
                 format.at(i + 1) == 'P')) {
                i++;
            }
            if (lastChar.cchar == c) {
                lastChar.repeat++;
            } else {
                lastChar.cchar  = c;
                lastChar.repeat = 1;
            }
        }
        if (lastChar.cchar != '\0') {
            append(lastChar);
        }
    }
};

QJalaliDateTime::QJalaliDateTime() : d_ptr(new QJalaliDateTimePrivate) {}

QJalaliDateTime::~QJalaliDateTime() {}

QJalaliDateTime::QJalaliDateTime(const QDateTime& datetime)
    : d_ptr(new QJalaliDateTimePrivate) {
    d_ptr->dateTime = datetime;
}
QJalaliDateTime::QJalaliDateTime(const QJalaliDateTime& datetime)
    : d_ptr(new QJalaliDateTimePrivate) {
    d_ptr->dateTime = datetime.toGregorian();
}

qint64
QJalaliDateTime::daysTo(const QJalaliDateTime& other) const {
    return d_ptr->dateTime.daysTo(other.toGregorian());
}

QJalaliDateTime
QJalaliDateTime::toUTC() const {
    return QJalaliDateTime{d_ptr->dateTime.toUTC()};
}

qint64
QJalaliDateTime::toSecsSinceEpoch() const {
    return d_ptr->dateTime.toSecsSinceEpoch();
}

QDateTime
QJalaliDateTime::toGregorian() const {
    return d_ptr->dateTime;
}

QString
QJalaliDateTime::toString(const QString& format, Numbers numbersView) const {
    auto    v = d_ptr->GregorianToJalali();
    QString result;
    bool    arabicFormat = false;
    if (numbersView == ArabicIndicNumbers ||
        (numbersView == DefaultNumbers &&
         d_ptr->defaultNumbersView == ArabicIndicNumbers))
        arabicFormat = true;

    CharVector charVec{format};
    auto       dt      = d_ptr->GregorianToJalali();
    auto       addyear = [&](int n) {
        while (n > 0) {
            if (n >= 4) {
                result.append(pad(dt[0], 4, arabicFormat));
                n -= 4;
            } else if (n >= 2) {
                result.append(pad(dt[0] % 100, 2, arabicFormat));
                n -= 2;
            } else if (n >= 1) {
                result.append('y');
                n--;
            }
        }
    };

    auto addDay = [&](int     number,
                      int     dayNumber,
                      int     n,
                      QString name(int, bool) = nullptr) {
        while (n > 0) {
            if (n >= 4 && name != nullptr) {
                result.append(name(dayNumber, false));
                n -= 4;
            } else if (n >= 3 && name != nullptr) {
                result.append(name(dayNumber, true));
                n -= 3;
            } else if (n >= 2) {
                result.append(pad(number, 2, arabicFormat));
                n -= 2;
            } else if (n >= 1) {
                result.append(pad(number, 1, arabicFormat));
                n--;
            }
        }
    };
    auto addOther = [&](int number, int n, QString name(int, bool) = nullptr) {
        while (n > 0) {
            if (n >= 4 && name != nullptr) {
                result.append(name(number, false));
                n -= 4;
            } else if (n >= 3 && name != nullptr) {
                result.append(name(number, true));
                n -= 3;
            } else if (n >= 2) {
                result.append(pad(number, 2, arabicFormat));
                n -= 2;
            } else if (n >= 1) {
                result.append(pad(number, 1, arabicFormat));
                n--;
            }
        }
    };
    auto addMilisecond = [&](int number, int n) {
        while (n > 0) {
            if (n >= 3) {
                result.append(
                    formatNumber(QString::number(number), arabicFormat));
                n -= 3;
            } else if (n >= 1) {
                result.append(
                    formatNumber(QString::number(number), arabicFormat));
                if (result.endsWith(formatNumber("0", arabicFormat)))
                    result.chop(1);
                if (result.endsWith(formatNumber("0", arabicFormat)))
                    result.chop(1);
                n--;
            }
        }
    };

    for (const auto& c : charVec) {
        switch (c.cchar.toLatin1()) {
        case 'y':
            addyear(c.repeat);
            break;
        case 'M':
            addOther(dt[1], c.repeat, nameOfMonth);
            break;
        case 'd':
            addDay(
                dt[2], d_ptr->dateTime.date().dayOfWeek(), c.repeat, nameOfDay);
            break;
        case 'h':
            addOther(
                charVec.containAp ? d_ptr->dateTime.time().hour() % 12
                                  : d_ptr->dateTime.time().hour(),
                c.repeat);
            break;
        case 'H':
            addOther(d_ptr->dateTime.time().hour(), c.repeat);
            break;
        case 'm':
            addOther(d_ptr->dateTime.time().minute(), c.repeat);
            break;
        case 's':
            addOther(d_ptr->dateTime.time().second(), c.repeat);
            break;
        case 'z':
            addMilisecond(d_ptr->dateTime.time().msec(), c.repeat);
            break;
        case 'a':
            result.append(d_ptr->dateTime.time().hour() > 12 ? bz : gz);
            break;
        case 't':
            result.append(
                formatNumber(d_ptr->dateTime.toString("t"), arabicFormat));
            break;
        default:
            result.append(repeat(c.cchar, c.repeat));
        }
    }

    return result;
}

QJalaliDateTime
QJalaliDateTime::fromGregorian(const QDateTime& datetime) {
    return QJalaliDateTime{datetime};
}

void
QJalaliDateTime::setDefaultNumberTextView(
    QJalaliDateTime::Numbers numbersView) {
    d_ptr->defaultNumbersView = numbersView;
}

#if !defined(QT_NO_DEBUG_STREAM) && QT_CONFIG(datestring)
QDebug
operator<<(QDebug dbg, const QJalaliDateTime& datetime) {
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "QJalaliDateTime(";
    dbg.noquote()
        << datetime.toString("yyyy/M/d h:m:s t", QJalaliDateTime::LatinNumbers);
    dbg.nospace() << ')';
    return dbg;
}
#endif

QJalaliDateTime
QJalaliDateTime::currentDateTime() {
    return QJalaliDateTime{QDateTime::currentDateTime()};
}

QJalaliDateTime QJalaliDateTime::fromJalaliDate(int year, int month, int day)
{
//    function jalali_to_gregorian(jy,jm,jd){
//        var gy=(jy<=979)?621:1600;
//        jy-=(jy<=979)?0:979;
//        var days=(365*jy) +((parseInt(jy/33))*8) +(parseInt(((jy%33)+3)/4))
//            +78 +jd +((jm<7)?(jm-1)*31:((jm-7)*30)+186);
//        gy+=400*(parseInt(days/146097));
//        days%=146097;
//        if(days > 36524){
//            gy+=100*(parseInt(--days/36524));
//            days%=36524;
//            if(days >= 365)days++;
//        }
//        gy+=4*(parseInt((days)/1461));
//        days%=1461;
//        gy+=parseInt((days-1)/365);
//        if(days > 365)days=(days-1)%365;
//        var gd=days+1;
//        var sal_a=[0,31,((gy%4==0 && gy%100!=0) || (gy%400==0))?29:28,31,30,31,30,31,31,30,31,30,31];
//        var gm
//            for(gm=0;gm<13;gm++){
//            var v=sal_a[gm];
//            if(gd <= v)break;
//            gd-=v;
//        }
//        return [gy,gm,gd];
//    }
}
