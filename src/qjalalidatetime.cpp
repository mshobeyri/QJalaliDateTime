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

    QVector<int> GregorianToJalali() {
        auto date = dateTime.date();
        return gregorianToJalali(date.year(), date.month(), date.day());
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

QJalaliDateTime::QJalaliDateTime(const QDateTime& gregorianDateime)
    : d_ptr(new QJalaliDateTimePrivate) {
    d_ptr->dateTime = gregorianDateime;
}
QJalaliDateTime::QJalaliDateTime(const QJalaliDateTime& datetime)
    : d_ptr(new QJalaliDateTimePrivate) {
    d_ptr->dateTime = datetime.toGregorian();
}

QJalaliDateTime::QJalaliDateTime(int jalaliYear, int jalaliMonth, int jalaliDay)
    : d_ptr(new QJalaliDateTimePrivate) {
    QVector<int> gdateVec =
        jalaliToGregorian(jalaliYear, jalaliMonth, jalaliDay);
    qDebug() << gdateVec;
    d_ptr->dateTime.setDate(QDate(gdateVec[0], gdateVec[1], gdateVec[2]));
    qDebug() << d_ptr->dateTime;
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

QJalaliDateTime
QJalaliDateTime::addDays(qint64 ndays) const {
    return d_ptr->dateTime.addDays(ndays);
}

QJalaliDateTime
QJalaliDateTime::addMSecs(qint64 msecs) const {
    return d_ptr->dateTime.addMSecs(msecs);
}

QJalaliDateTime
QJalaliDateTime::addMonths(int nmonths) const {
    return d_ptr->dateTime.addMonths(nmonths);
}

QJalaliDateTime
QJalaliDateTime::addSecs(qint64 s) const {
    return d_ptr->dateTime.addSecs(s);
}

QJalaliDateTime
QJalaliDateTime::addYears(int nyears) const {
    return d_ptr->dateTime.addYears(nyears);
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

QJalaliDateTime
QJalaliDateTime::fromGregorianString(
    const QString& string, Qt::DateFormat format) {
    return QDateTime::fromString(string, format);
}

QJalaliDateTime
QJalaliDateTime::fromGregorianString(
    const QString& string, const QString& format) {
    return QDateTime::fromString(string, format);
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

QJalaliDateTime
QJalaliDateTime::currentDateTimeUtc() {
    return QJalaliDateTime{QDateTime::currentDateTimeUtc()};
}

QJalaliDateTime
QJalaliDateTime::fromJalaliDate(
    int jalaliYear, int jalaliMonth, int jalaliDay) {
    return QJalaliDateTime{jalaliYear, jalaliMonth, jalaliDay};
}

QJalaliDateTime
QJalaliDateTime::fromMSecsSinceEpoch(qint64 gregorianMsecs) {
    return QJalaliDateTime{QDateTime::fromMSecsSinceEpoch(gregorianMsecs)};
}

QJalaliDateTime
QJalaliDateTime::fromSecsSinceEpoch(qint64 gregorianSecs) {
    return QJalaliDateTime{QDateTime::fromSecsSinceEpoch(gregorianSecs)};
}
