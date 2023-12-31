#include "controller.h"

#include "sqlitescreenshotsdb.h"

#include <QTimer>
#include <QBuffer>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QPair>
#include <QDebug>
#include <QMutexLocker>

#include <algorithm>

const QString Controller::DbPath = "./images.db";

Controller::Controller(QObject *parent)
    : QObject{parent}
    , m_db{std::make_unique<SQLiteScreenshotsDb>(DbPath)}
    , m_workerThread {
          [&] {
              while (m_runningWorker) {
                QMutexLocker l(&m_screenshotMutex);
                if (m_comparingScreenshot) {
                      auto s = *m_comparingScreenshot;
                      setComparisonPercentage(s);

                      emit screenshotProcessed(s);
                      m_comparingScreenshot = nullptr;
                }
              }
          }
      }
{
    connect(this, &Controller::screenshotProcessed, [this](const Screenshot& s) {
        screenshotsList.appendFront(s);
        emit screenshotsChanged();

        m_updatedShots.append(&s);
        emit screenshotSaved();
    });

    connect(&m_shotTimer, &QTimer::timeout, this, &Controller::makeScreenshot);

    fillList();
}

Controller::~Controller()
{
    m_runningWorker = false;
    m_workerThread.join();
}

void Controller::run()
{
    m_shotTimer.start(ScreenshotDelay * 1000);
}

void Controller::stop()
{
    m_shotTimer.stop();
}

void Controller::makeScreenshot()
{
    requireDbUpdated();

    QScreen *screen = QGuiApplication::primaryScreen();

    if (!screen)
        return;

    auto pixmap = screen->grabWindow(0);

    emit screenShooted();

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QMutexLocker l(&m_screenshotMutex);
    m_comparingScreenshot = std::make_unique<Screenshot>();

    m_comparingScreenshot->comparisonPercentage = 0.0;
    m_comparingScreenshot->hashData = bytes.toBase64();
    m_comparingScreenshot->takenTime = QDateTime::currentDateTime();

}

void Controller::fillList()
{
    auto items = m_db->items();

    std::sort(items.begin(), items.end(),
              [](const auto &left, const auto &right) {
                  return left.takenTime > right.takenTime;
              });

    for (const auto& item : items)
        screenshotsList.append(item);
}

void Controller::setComparisonPercentage(Screenshot& s)
{
    auto& items = screenshotsList.items();
    s.comparisonPercentage = items.empty() ? -1 : 100.f * calcIdentity(s, items.front());
}

void Controller::requireDbUpdated()
{
    QMutexLocker l(&m_screenshotMutex);

    for (auto s : m_updatedShots)
        m_db->insert(*s);

    m_updatedShots.clear();
}

static int countSetBits(char c) {
    int count = 0;

    for (; c; c >>= 1)
        count += c & 1;

    return count;
}

float Controller::calcIdentity(const Screenshot &current, const Screenshot &last)
{
    using CharPair = QPair<char, char>;
    constexpr int EncodingBase = 64;

    auto currentRaw = current.hashData;
    auto lastRaw = last.hashData;

    const int len = std::min(lastRaw.size(), currentRaw.size());
    const int lenDiff = std::abs(lastRaw.size() - currentRaw.size());

    int diff = EncodingBase * lenDiff;

    for (int i = 0; i < len; ++i) {
        char difference = currentRaw.at(i) ^ lastRaw.at(i);

        if (difference)
            diff += countSetBits(difference);
    }

    return 1.0f - float(diff) / EncodingBase / (len + lenDiff);
}

ScreenshotsList* Controller::screenshots()
{
    return &screenshotsList;
}

void Controller::setScreenshots(ScreenshotsList *newScreenshotsList)
{
    screenshotsList = *newScreenshotsList;
}
