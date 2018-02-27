#ifndef COMMANDFILTER_H
#define COMMANDFILTER_H

#include <QSortFilterProxyModel>
#include <QModelIndex>

class CommandFilter : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    CommandFilter(QObject* parent = nullptr);
    ~CommandFilter();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public:
    void invalidateFilter();
};

#endif // COMMANDFILTER_H
