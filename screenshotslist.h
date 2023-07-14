#ifndef SCREENSHOTSLIST_H
#define SCREENSHOTSLIST_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>
#include <QVector>
#include <QMutex>

#include "screenshot.h"

class ScreenshotsList : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotsList(QObject *parent = nullptr);

    ScreenshotsList& operator=(const ScreenshotsList&);

    const QVector<Screenshot>& items() const;
    void append(const Screenshot& item);
    void appendFront(const Screenshot& item);
    void setAt(int index, Screenshot item);

signals:
    void preItemAppended();
    void postItemAppended();

private:
    QVector<Screenshot> m_screenshots;
    mutable QMutex m_mutex;
};

#endif // SCREENSHOTSLIST_H
