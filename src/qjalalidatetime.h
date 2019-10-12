#ifndef QJALALIDATETIME_H
#define QJALALIDATETIME_H

#include <QDateTime>
#include <QObject>
#include <QScopedPointer>

#if QmlPorject
class QQmlApplicationEngine;
#endif
class QJalaliDateTimePrivate;
class QJalaliDateTime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gyear READ getGYear WRITE setGYear)
    Q_PROPERTY(int gmonth READ getGMonth WRITE setGMonth)
    Q_PROPERTY(int gday READ getGDay WRITE setGDay)
    Q_PROPERTY(int jyear READ getJYear WRITE setJYear)
    Q_PROPERTY(int jmonth READ getJMonth WRITE setJMonth)
    Q_PROPERTY(int jday READ getJDay WRITE setJDay)
public:
    enum Numbers { ArabicIndicNumbers, LatinNumbers, DefaultNumbers };

#if QmlPorject
    Q_ENUM(Numbers);
#endif

    QJalaliDateTime();
    QJalaliDateTime(const QDateTime& gregorianDateime);
    QJalaliDateTime(const QJalaliDateTime& datetime);
    QJalaliDateTime(int jalaliYear, int jalaliMonth, int jalaliDay);
    ~QJalaliDateTime();

    qint64 daysTo(const QJalaliDateTime& other) const;

    QJalaliDateTime toUTC() const;
    qint64          toSecsSinceEpoch() const;
    QDateTime       toGregorian() const;


    QJalaliDateTime addDays(qint64 ndays) const;
    QJalaliDateTime addMSecs(qint64 msecs) const;
    QJalaliDateTime addMonths(int nmonths) const;
    QJalaliDateTime addSecs(qint64 s) const;
    QJalaliDateTime addYears(int nyears) const;

    static QJalaliDateTime currentDateTime();
    static QJalaliDateTime currentDateTimeUtc();

    static QJalaliDateTime
                           fromJalaliDate(int jalaliYear, int jalaliMonth, int jalaliDay);
    static QJalaliDateTime fromMSecsSinceEpoch(qint64 gregorianMsecs);
    static QJalaliDateTime fromSecsSinceEpoch(qint64 gregorianSecs);
    static QJalaliDateTime fromGregorian(const QDateTime& datetime);

    static QJalaliDateTime fromGregorianString(
        const QString& string, Qt::DateFormat format = Qt::TextDate);
    static QJalaliDateTime
    fromGregorianString(const QString& string, const QString& format);

    void
    setDefaultNumberTextView(Numbers numbersView = Numbers::ArabicIndicNumbers);

    void updateByJalaliDate(int jalaliYear, int jalaliMonth, int jalaliDay);

    Q_INVOKABLE QString toString(
        const QString& format      = QString("yyyy/MM/dd hh:mm"),
        Numbers        numbersView = Numbers::DefaultNumbers) const;

    int  getGYear();
    void setGYear(int year);
    int  getGMonth();
    void setGMonth(int month);
    int  getGDay();
    void setGDay(int day);
    int  getJYear();
    void setJYear(int year);
    int  getJMonth();
    void setJMonth(int month);
    int  getJDay();
    void setJDay(int day);


#if !defined(QT_NO_DEBUG_STREAM) && QT_CONFIG(datestring)
    friend Q_CORE_EXPORT QDebug
                         operator<<(QDebug dbg, const QJalaliDateTime& message);
#endif
#if QmlPorject
    static void registerQml(QQmlApplicationEngine& engine);
#endif

private:
    Q_DECLARE_PRIVATE(QJalaliDateTime)
    QScopedPointer<QJalaliDateTimePrivate> d_ptr;
};

#endif // QJALALIDATETIME_H
