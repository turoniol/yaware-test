#include "screenshotslist.h"

ScreenshotsList::ScreenshotsList(QObject *parent)
    : QObject{parent}
{

}

const QVector<Screenshot> &ScreenshotsList::items() const
{
    return m_screenshots;
}
