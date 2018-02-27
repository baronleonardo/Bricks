#include "commandfilter.h"

CommandFilter::CommandFilter(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool CommandFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex indexCol0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex indexCol1 = sourceModel()->index(sourceRow, 1, sourceParent);

    return sourceModel()->data(indexCol0).toString().contains(filterRegExp()) ||
            sourceModel()->data(indexCol1).toString().contains(filterRegExp());
}

void CommandFilter::invalidateFilter() {
    QSortFilterProxyModel::invalidateFilter();
}

CommandFilter::~CommandFilter() {

}
