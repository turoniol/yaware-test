#ifndef SCREENSHOTSMODEL_H
#define SCREENSHOTSMODEL_H

#include <QAbstractListModel>

class ScreenshotsList;

class ScreenshotsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ScreenshotsList *screenshots READ screenshots WRITE setScreenshots)
public:
    enum {
        PercentageRole = Qt::UserRole + 1,
        HashDataRole,
        DateRole,
        TimeRole
    };

    explicit ScreenshotsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    ScreenshotsList* screenshots() const;
    void setScreenshots(ScreenshotsList *list);

    QHash<int, QByteArray> roleNames() const;

private:
    ScreenshotsList* m_screenshots = nullptr;
};

#endif // SCREENSHOTSMODEL_H
