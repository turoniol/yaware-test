#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include <memory>

#include "screenshotsdbbase.h"
#include "screenshot.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

signals:
    void screenShooted();
    void screenshotProcessed(const Screenshot&);
    void screenshotSaved();

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
};

#endif // CONTROLLER_H
