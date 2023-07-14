#include "screenshotsdbbase.h"

#include <QDebug>

void ScreenshotsDbBase::open(const QString& dbPath)
{
    m_db = QSqlDatabase::addDatabase(dbType());
    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
        qDebug() << "Error: connection with database failed";
    else
        qDebug() << "Database: connection ok";
}

ScreenshotsDbBase::~ScreenshotsDbBase() {
    m_db.close();
}

void ScreenshotsDbBase::insert(const Screenshot &item)
{
    emit preItemInserted();
    insertInternal(item);
    emit postItemInserted();
}
