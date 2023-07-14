#ifndef SCREENSHOTSLIST_H
#define SCREENSHOTSLIST_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>
#include <QVector>

#include "screenshot.h"

class ScreenshotsList : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotsList(QObject *parent = nullptr);

    const QVector<Screenshot>& items() const;
    void append(const Screenshot& item);
    void setAt(int index, Screenshot item);

signals:
    void preItemAppended();
    void postItemAppended();

private:
    QVector<Screenshot> m_screenshots;
};

#endif // SCREENSHOTSLIST_H
