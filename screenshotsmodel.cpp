#include "screenshotsmodel.h"

#include "screenshotslist.h"

ScreenshotsModel::ScreenshotsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ScreenshotsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant ScreenshotsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool ScreenshotsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ScreenshotsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

ScreenshotsList *ScreenshotsModel::screenshots() const
{
    return m_screenshots;
}

void ScreenshotsModel::setScreenshots(ScreenshotsList *list)
{
    beginResetModel();

    if (m_screenshots)
        m_screenshots->disconnect(this);

    m_screenshots = list;

    endResetModel();
}
