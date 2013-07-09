#ifndef QNETSTRING_H
#define QNETSTRING_H

#include <QObject>
#include <QAbstractSocket>

class QNetString : public QObject
{
    Q_OBJECT
public:
    explicit QNetString(QObject *parent = 0);

    static QByteArray encode(QByteArray data);

private:
    QByteArray buffer;

signals:
    void dataReady(QByteArray);
    void tryParse();

private slots:
    void onTryParse();

public slots:
    void addData(QByteArray data);
};

#endif // QNETSTRING_H
