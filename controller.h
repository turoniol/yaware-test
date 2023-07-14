#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>

#include <memory>
#include <thread>
#include <QThread>

#include "screenshotsdbbase.h"
#include "screenshot.h"
#include "screenshotslist.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ScreenshotsList *screenshots READ screenshots WRITE setScreenshots NOTIFY screenshotsChanged)
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    ScreenshotsList* screenshots();
    void setScreenshots(ScreenshotsList *newScreenshotsList);

signals:
    void screenShooted();
    void screenshotProcessed(const Screenshot&);
    void screenshotSaved();
    void screenshotsChanged();

public slots:
    void run();
    void stop();

private slots:
    void makeScreenshot();

private:
    void fillList();
    void setComparisonPercentage(Screenshot& s);
    void requireDbUpdated();

    static float calcIdentity(const Screenshot& current, const Screenshot& last);

    static constexpr int ScreenshotDelay = 2; // seconds
    static const QString DbPath;

    std::unique_ptr<ScreenshotsDbBase> m_db;
    ScreenshotsList screenshotsList;
    QTimer m_shotTimer;

    QVector<const Screenshot *> m_updatedShots;

    std::thread m_workerThread;
    std::atomic<bool> m_runningWorker = true;
    std::unique_ptr<Screenshot> m_comparingScreenshot = nullptr;
    QMutex m_screenshotMutex;
};

#endif // CONTROLLER_H
