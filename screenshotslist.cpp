#include "screenshotslist.h"

ScreenshotsList::ScreenshotsList(QObject *parent)
    : QObject{parent}
{

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

void ScreenshotsList::setAt(int index, Screenshot item)
{
    m_screenshots.replace(index, item);
}

