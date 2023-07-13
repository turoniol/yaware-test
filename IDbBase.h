#ifndef ISCREENSHOTDB_H
#define ISCREENSHOTDB_H

#include <QVector>
#include <QString>
#include <QSqlDatabase>


template <class T>
struct IDbBase {
    IScreenshotDb(const QString& dbPath) {
        m_db = QSqlDatabase::addDatabase(dbType());
        m_db.setDatabaseName(dbPath);

        if (!m_db.open())
            qDebug() << "Error: connection with database failed";
        else
            qDebug() << "Database: connection ok";
    }
    virtual ~IScreenshotDb() {
        m_db.close();
    }

    virtual QVector<T> items() const = 0;
    void insert(const T& item) {
        emit preItemInserted();
        insertInternal(item);
        emit postItemInserted();
    }

signals:
    void preItemInserted();
    void postItemInserted();

protected:
    virtual QString dbType() const = 0;
    virtual void insertInternal(const T& item) = 0;

private:
    QSqlDatabase m_db;
};

#endif // ISCREENSHOTDB_H
