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
    if (parent.isValid() || !m_screenshots)
        return 0;

    return m_screenshots->items().size();
}

QVariant ScreenshotsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_screenshots)
        return QVariant();

    const auto& item = m_screenshots->items().at(index.row());
    switch (role) {
    case PercentageRole:
        return QVariant(item.comparisonPercentage);
    case HashDataRole:
        return QVariant(QString(item.hashData));
    case DateRole:
        return QVariant(item.takenTime.date().toString());
    case TimeRole:
        return QVariant(item.takenTime.time().toString());
    default:
        return QVariant();
    }
}

bool ScreenshotsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_screenshots)
        return false;

    if (data(index, role) != value) {
        auto item = m_screenshots->items().at(index.row());

        switch (role) {
        case PercentageRole:
            item.comparisonPercentage = value.toFloat();
            break;
        case HashDataRole:
            item.hashData = value.toByteArray();
            break;
        case DateRole:
            item.takenTime.setDate(value.toDate());
            break;
        case TimeRole:
            item.takenTime.setTime(value.toTime());
            break;
        }

        m_screenshots->setAt(index.row(), item);

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ScreenshotsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
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

    if (m_screenshots) {
        connect(m_screenshots, &ScreenshotsList::preItemAppended, [this]{
            beginInsertRows(QModelIndex(), 0, 0);
        });
        connect(m_screenshots, &ScreenshotsList::postItemAppended, [this] {
            endInsertRows();
        });
    }

    endResetModel();
}

QHash<int, QByteArray> ScreenshotsModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[PercentageRole] = "percentage";
    result[HashDataRole] = "hashData";
    result[DateRole] = "date";
    result[TimeRole] = "time";
    return result;
}
