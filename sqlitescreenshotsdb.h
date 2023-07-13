#ifndef SQLITESCREENSHOTSDB_H
#define SQLITESCREENSHOTSDB_H

#include "IDbBase.h"
#include "screenshot.h"

QT_BEGIN_NAMESPACE
class QSqlRecord;
QT_END_NAMESPACE

class SQLiteScreenshotsDb final : IDbBase<Screenshot>
{
public:
    SQLiteScreenshotsDb(const QString& dbPath);

    QVector<Screenshot> items() const override;

protected:
    void insertInternal(const Screenshot& item) override;

    QString dbType() const;

private:
    Screenshot parse(const QSqlRecord& record);
};

#endif // SQLITESCREENSHOTSDB_H
