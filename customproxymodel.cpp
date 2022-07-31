#include "customproxymodel.h"

#include <utility>

CustomProxyModel::CustomProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void CustomProxyModel::setMinDatetime(const QDateTime& dateTime)
{
    minDateTime = dateTime;
    invalidateFilter();
}

void CustomProxyModel::setMaxDateTime(const QDateTime& dateTime)
{
    maxDateTime = dateTime;
    invalidateFilter();
}

void CustomProxyModel::setTimeColumn(int col)
{
    timeCol = col;
}

void CustomProxyModel::setSendColumn(int col)
{
    sendCol = col;
}

void CustomProxyModel::setTypeColumn(int col)
{
    typeCol = col;
}

bool CustomProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, timeCol, sourceParent); // 时间列
    QModelIndex index1 = sourceModel()->index(sourceRow, sendCol, sourceParent); // 接收发送列
    QModelIndex index2 = sourceModel()->index(sourceRow, typeCol, sourceParent); // 类型列

    return (sourceModel()->data(index2).toString().contains(filterRegularExpression())
            && sourceModel()->data(index1).toString().contains(filterRegularExpression()))
            && dateInRange(QDateTime::fromString(sourceModel()->data(index0).toString(), "yyyy-MM-dd hh:mm:ss.zzz"));
}

bool CustomProxyModel::dateInRange(const QDateTime& dateTime) const
{
    return (!minDateTime.isValid() || dateTime > minDateTime)
            && (!maxDateTime.isValid() || dateTime < maxDateTime);
}
