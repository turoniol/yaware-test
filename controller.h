#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include <memory>

#include "screenshotsdbbase.h"
#include "screenshot.h"
#include "screenshotslist.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ScreenshotsList *screenshots READ screenshots WRITE setScreenshots NOTIFY screenshotsChanged)
public:
    explicit Controller(QObject *parent = nullptr);

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
    void setComparisonPercentage(Screenshot& s);

    static float calcIdentity(const Screenshot& current, const Screenshot& last);

    static constexpr int ScreenshotDelay = 60;
    static const QString DbPath;

    std::unique_ptr<ScreenshotsDbBase> m_db;
    ScreenshotsList screenshotsList;
};

#endif // CONTROLLER_H
