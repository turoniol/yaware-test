#include "screenshotslist.h"

#include <QMutexLocker>

ScreenshotsList::ScreenshotsList(QObject *parent)
    : QObject{parent}
{}

ScreenshotsList &ScreenshotsList::operator=(const ScreenshotsList &s)
{
    QMutexLocker l(&m_mutex);
    m_screenshots = s.m_screenshots;
    return *this;
}

const QVector<Screenshot> &ScreenshotsList::items() const
{
    QMutexLocker l(&m_mutex);
    return m_screenshots;
}

void ScreenshotsList::append(const Screenshot &item)
{
    emit preItemAppended();

    QMutexLocker l(&m_mutex);
    m_screenshots << item;
    emit postItemAppended();
}

void ScreenshotsList::appendFront(const Screenshot &item)
{
    QMutexLocker l(&m_mutex);
    m_screenshots.push_front(item);
}

void ScreenshotsList::setAt(int index, Screenshot item)
{
    QMutexLocker l(&m_mutex);
    m_screenshots.replace(index, item);
}

