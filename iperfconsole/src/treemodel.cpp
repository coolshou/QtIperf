#include "treemodel.h"
#include "treeitem.h"

#include <QStringList>

#include <QDebug>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Group" << "Status" <<
                "Endpoint1" << "<>" << "Endpoint2" <<
                "Protocal" << "Throughput" << "comment";
    rootItem = new TreeItem(rootData);


}
TreeModel::~TreeModel()
{
    delete rootItem;
}
int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    //TODO: show data by column
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
    /*
    if (role == Qt::DisplayRole) {

        //return rootItem->data(section);
        switch (index.column()){
        case Column::group:
            //TODO: Column::group:
            break;
        case Column::status:
            //TODO: Column::status:
            break;
        case Column::endpoint1:
            //TODO: Column::endpoint1:
            return item->data(index.column());
            break;
        case Column::direction:
            //TODO: Column::direction:
            break;
        case Column::endpoint2:
            //TODO: Column::endpoint2:
            break;
        case Column::protocal:
            //TODO: Column::protocal:
            break;
        case Column::throughput:
            //TODO: Column::throughput:
            break;
        case Column::comment:
            //TODO: Column::comment:
            break;
        default:
            return QVariant();
            break;
        }


    }*/



}
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    //show column header
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        /*
        switch (section) {
        case Column::group:
            return "Group";
            break;
        case Column::status:
            return "Status";
            break;
        case Column::endpoint1:
            return "Endpoint1";
            break;
        case Column::direction:
            return "<>";
            break;
        case Column::endpoint2:
            return "Endpoint2";
            break;
        case Column::protocal:
            return "Protocal";
            break;
        case Column::throughput:
            return "Throughput";
            break;
        case Column::comment:
            return "comment";
            break;
        default:
            break;
        }
        */
        return rootItem->data(section);

    return QVariant();
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//int TreeModel::addPair(int idx, QString e1, QString e2, int iRun, QString iperfargs, QString m1, QString m2)
int TreeModel::addPair(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2)
{
    //TODO: TreeItem columns
    QList<QVariant> data;
    //TODO: iRun << iperfargs << m1 << m2
    //TODO: direction
    data << "" << "" << e1 << "=>" << e2 << "TCP" << "" << "";
    TreeItem *child = new TreeItem(data,rootItem);

    rootItem->appendChild(child);
    qDebug() << "root childs: "<< rootItem->childCount();
}
