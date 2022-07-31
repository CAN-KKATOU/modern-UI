#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDateTime>

class CustomProxyModel : public QSortFilterProxyModel
{
public:
    explicit CustomProxyModel(QObject *parent = nullptr);

    void setMinDatetime(const QDateTime& dateTime);

    void setMaxDateTime(const QDateTime& dateTime);

    void setTimeColumn(int);

    void setSendColumn(int);

    void setTypeColumn(int);

    [[nodiscard]] QDateTime getMaxDateTime() const { return maxDateTime; }

    [[nodiscard]] QDateTime getMinDateTime() const { return minDateTime; }


protected:

    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:

    [[nodiscard]] bool dateInRange(const QDateTime& dateTime) const;

    int typeCol = 0;

    int timeCol = 0;

    int sendCol = 0;

    QDateTime minDateTime;

    QDateTime maxDateTime;
};

#endif // CUSTOMPROXYMODEL_H
