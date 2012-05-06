#include "treeitem.h"
#include "treemodel.h"
#include <QDebug>
#include <QIcon>

TreeModel::TreeModel(QList<QVariant> rootData, TreeItemInfo* treeItemsInfo, QObject *parent) :
    QAbstractItemModel(parent),
    itemInfo(treeItemsInfo),
    columnsTotal(rootData.count())
{
    init(rootData);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::init(QList<QVariant> rootData)
{
    rootItem = new TreeItem(rootData);
    setupModelData(rootItem);
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

    if (role == Qt::DecorationRole)
    {
        //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! item" << itemInfo->items[getCurrentModelItemIndex(static_cast<TreeItem*>(index.internalPointer()))].name;
        return QIcon(itemInfo->items[getCurrentModelItemIndex(static_cast<TreeItem*>(index.internalPointer()))].iconPath);
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
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
    TreeItem *parentItem = childItem->parent();

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

void TreeModel::setupModelData(TreeItem *parent)
{
    buildTree(ROOT_FOLDER, parent);
}

void TreeModel::buildTree(const QString& searchStr, TreeItem *parent)
{
    QList< QList<QVariant> > columnData;
    QList<QVariant> selfs;
    QList<int> indexes;

    int count = 0;

    for (int i = itemInfo->items.count() -1; i >=0 ; --i)
    {
        if(itemInfo->items[i].parent.toString() == searchStr)
        {
            QList<QVariant> column;

            column.push_back(itemInfo->items[i].name);
            //column.push_back(itemInfo->items[i].self);
            columnData.push_back(column);
            selfs.push_back(itemInfo->items[i].self);
            indexes.push_back(i);
            ++count;
        }
    }

    if(count > 0)
    {
        fillTree(columnData, parent, indexes);

        for (int i = count - 1; i >= 0; --i)
        {
            buildTree(selfs[i].toString(), parent->child(i));
        }
    }
}

void TreeModel::fillTree(QList< QList<QVariant> > columnData, TreeItem *parent, QList<int> indexes)
{
    for (int i = 0; i < columnData.count(); ++i)
    {
        TreeItem* item = new TreeItem(columnData[i], parent);
        parent->appendChild(item);
        itemInfo->setItemPointer(indexes[i], item);
        qDebug() << "------------------> item[" << indexes[i] << "] = " << item;
        //parent->appendChild(new TreeItem(columnData[i], parent));
    }
}

//bool TreeModel::allowDraw(const QModelIndex &index) const
//{
//    static void* prevInternalPointer = NULL;
//    void* currentInternalPointer = index.internalPointer();
//    bool allow = false;

//    if(currentInternalPointer!= prevInternalPointer) allow = true;
//    prevInternalPointer = currentInternalPointer;

//    qDebug() << "------------------> allow = " << allow << "currentInternalPointer = " << currentInternalPointer;

//    return allow;
//}

int TreeModel::getCurrentModelItemIndex(TreeItem *item) const
{
    int currentModelIndex = 0;

    for(int i = 1; i < itemInfo->items.count(); ++i)
    {
        if(itemInfo->items[i].item == item)
        {
            currentModelIndex = i;
            break;
        }
    }

    return currentModelIndex;
}
