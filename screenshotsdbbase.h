#ifndef ISCREENSHOTDB_H
#define ISCREENSHOTDB_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QSqlDatabase>

struct Screenshot;

class ScreenshotsDbBase : public QObject {
    Q_OBJECT

public:
    void open(const QString& dbPath);
    virtual ~ScreenshotsDbBase();

    virtual QVector<Screenshot> items() const = 0;
    void insert(const Screenshot& item);

signals:
    void preItemInserted();
    void postItemInserted();

protected:
    virtual QString dbType() const = 0;
    virtual void insertInternal(const Screenshot& item) = 0;

private:
    QSqlDatabase m_db;
};

#endif // ISCREENSHOTDB_H
