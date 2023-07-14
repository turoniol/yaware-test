#include "screenshotslist.h"

ScreenshotsList::ScreenshotsList(QObject *parent)
    : QObject{parent}
{

}

ScreenshotsList &ScreenshotsList::operator=(const ScreenshotsList &s)
{
    m_screenshots = s.m_screenshots;
    return *this;
}

const QVector<Screenshot> &ScreenshotsList::items() const
{
    return m_screenshots;
}

void ScreenshotsList::append(const Screenshot &item)
{
    emit preItemAppended();
    m_screenshots << item;
    emit postItemAppended();
}

void ScreenshotsList::appendFront(const Screenshot &item)
{
    m_screenshots.push_front(item);
}

void ScreenshotsList::setAt(int index, Screenshot item)
{
    m_screenshots.replace(index, item);
}

