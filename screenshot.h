#ifndef SCREENSHOT_H
#define SCREENSHOT_H

struct Screenshot {
    float comparisonPercentage;
    QByteArray rawData;
    QDateTime takenTime;
};

#endif // SCREENSHOT_H
