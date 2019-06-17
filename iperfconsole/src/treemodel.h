#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column {
        group=0,
        status,
        endpoint1,
        direction,
        endpoint2,
        protocal,
        throughput,
        comment
    };
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    //int addPair(int idx, QString e1, QString e2, int iRun, QString iperfargs, QString m1, QString m2);
    int addPair(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2);

private:
    TreeItem *rootItem;
};

#endif // TREEMODEL_H
