#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QDateTime>
#include <QByteArray>

struct Screenshot {
    float comparisonPercentage;
    QByteArray hashData;
    QDateTime takenTime;
};

#endif // SCREENSHOT_H
