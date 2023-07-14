#include "controller.h"

#include "sqlitescreenshotsdb.h"

#include <QTimer>
#include <QThread>
#include <QBuffer>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QPair>

#include <algorithm>

const QString Controller::DbPath = "./images.db";

Controller::Controller(QObject *parent)
    : QObject{parent}
    , m_db{std::make_unique<SQLiteScreenshotsDb>(DbPath)}
{ }

void Controller::run()
{
    QTimer::singleShot(ScreenshotDelay * 1000, this, &Controller::makeScreenshot);
}

void Controller::stop()
{
    assert(false && "don't forget to implement me");
}

void Controller::makeScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();

    if (!screen)
        return;

    auto pixmap = screen->grabWindow(0);

    emit screenShooted();

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    Screenshot screenshot{};

    screenshot.comparisonPercentage = 0.0;
    screenshot.hashData = bytes.toBase64();
    screenshot.takenTime = QDateTime::currentDateTime();

    emit screenshotProcessed(screenshot);

    m_db->insert(screenshot);

    emit screenshotSaved();
}

void Controller::setComparisonPercentage(Screenshot& s)
{
    auto items = m_db->items();

    std::sort(items.begin(), items.end(),
              [](const auto &left, const auto &right) {
                  return left.takenTime > right.takenTime;
              });

    s.comparisonPercentage = items.empty() ? -1 : calcIdentity(s, items.front());
}

static float countDiffs(const QVector<QPair<char, char>>& pairs) {
    int count = 0;

    for (const auto& pair : pairs) {
        QByteArray larr = QByteArray::fromBase64(QByteArray(1, pair.first));
        QByteArray rarr = QByteArray::fromBase64(QByteArray(1, pair.second));

        assert(larr.size() == rarr.size());

        for (int i = 0; i < larr.size(); ++i)
            if (larr.at(i) != rarr.size())
                ++count;
    }

    return count;
}

float Controller::calcIdentity(const Screenshot &current, const Screenshot &last)
{
    auto& currentHash = current.hashData;
    auto lastHash = last.hashData;


    if (lastHash.length() != currentHash.length())
        return 0.0f;

    using CharPair = QPair<char, char>;
    QVector<CharPair> diff;

    const int len = currentHash.length();

    for (int i = 0; i < len; ++i) {
        char cchar = currentHash.at(i);
        char lchar = lastHash.at(i);

        if (cchar != lchar)
            diff << CharPair(cchar, lchar);
    }

    return diff.empty() ? 1.0f : 1 - countDiffs(diff) / len;
}
