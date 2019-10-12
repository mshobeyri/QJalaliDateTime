#ifndef QJALALIDATETIME_H
#define QJALALIDATETIME_H

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QScopedPointer>

class QJalaliDateTimePrivate;
class QJalaliDateTime : public QObject
{
public:
    QJalaliDateTime();
    ~QJalaliDateTime();

    enum Numbers { ArabicIndicNumbers, LatinNumbers, DefaultNumbers };

    QJalaliDateTime(const QDateTime& gregorianDateime);
    QJalaliDateTime(const QJalaliDateTime& datetime);
    QJalaliDateTime(int jalaliYear, int jalaliMonth, int jalaliDay);
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

    static QJalaliDateTime fromJalaliDate(int year, int month, int day);
    static QJalaliDateTime fromMSecsSinceEpoch(qint64 gregorianMsecs);
    static QJalaliDateTime fromSecsSinceEpoch(qint64 gregorianSecs);
    static QJalaliDateTime fromGregorian(const QDateTime& datetime);

    static QJalaliDateTime fromGregorianString(
        const QString& string, Qt::DateFormat format = Qt::TextDate);
    static QJalaliDateTime
                           fromGregorianString(const QString& string, const QString& format);

    void
    setDefaultNumberTextView(Numbers numbersView = Numbers::ArabicIndicNumbers);


    QString toString(
        const QString& format,
        Numbers        numbersView = Numbers::DefaultNumbers) const;

#if !defined(QT_NO_DEBUG_STREAM) && QT_CONFIG(datestring)
    friend Q_CORE_EXPORT QDebug
                         operator<<(QDebug dbg, const QJalaliDateTime& message);
#endif

private:
    Q_DECLARE_PRIVATE(QJalaliDateTime)
    QScopedPointer<QJalaliDateTimePrivate> d_ptr;
};

#endif // QJALALIDATETIME_H
