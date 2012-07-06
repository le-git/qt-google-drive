#include "operationsui.h"

OperationsUI::OperationsUI(QObject *parent) :
    QObject(parent)
{
}

void OperationsUI::slotDel(QObject* object)
{
    if (object == SUi::inst()->foldersView)
    {
        if(!SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused])
        {
            TreeItemInfo item = *SDriveEngine::inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();

            if(item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].parent != "")
            {
                connect(SDriveEngine::inst()->foldersManager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
                SDriveEngine::inst()->foldersManager->del(item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].self);
                delItemInTree(item);
            }
        }
    }

    if (object == SUi::inst()->filesView)
    {
        FilesManager* manager;

        if(SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused]) manager = SDriveEngine::inst()->additionalFilesManager.data();
        else manager = SDriveEngine::inst()->filesManager.data();

        QList<TreeItemInfo::Data> itemData = manager->getParser()->getXMLHandler()->getTreeItemInfo()->getFileItems();

        connect(manager->getOperationsManager(), SIGNAL(signalDelFinished()), this, SLOT(slotDelFinished()));
        manager->del(itemData[SDriveEngine::inst()->filesUI->getCurrentFileItemIndex(manager)].self);
    }
}

void OperationsUI::delItemInTree(TreeItemInfo item)
{
    QTreeWidgetItem *parent = item[SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex()].pointer->parent();
    int index;

    if (parent)
    {
        index = parent->indexOfChild(SUi::inst()->foldersView->currentItem());
        delete parent->takeChild(index);
    }
    else
    {
        index = SUi::inst()->foldersView->indexOfTopLevelItem(SUi::inst()->foldersView->currentItem());
        delete SUi::inst()->foldersView->takeTopLevelItem(index);
    }
}

void OperationsUI::slotTriggeredDel()
{
    if(SDriveEngine::inst()->elementsStates[DriveEngine::ETrashFocused]) return;
    if(SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused] && !SDriveEngine::inst()->elementsStates[DriveEngine::EFilesViewFocused]) return;

    QObject* object;

    if(SDriveEngine::inst()->elementsStates[DriveEngine::EFolderViewFocused]) object = SUi::inst()->foldersView;
    else object = SUi::inst()->filesView;

    slotDel(object);
}

void OperationsUI::slotDelFinished()
{
    if(SDriveEngine::inst()->elementsStates[DriveEngine::EAdditionalViewFocused]) SDriveEngine::inst()->filesUI->slotAdditionalShowFiles(SDriveEngine::inst()->currentAdditionalFolderIndex);
    else SDriveEngine::inst()->filesUI->showFiles();
}

void OperationsUI::slotCreateFolder()
{
    SDriveEngine::inst()->createFolderDialog = new CreateFolderDialog(SDriveEngine::inst()->parent);

    connect(SDriveEngine::inst()->createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(SDriveEngine::inst()->createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(SDriveEngine::inst()->createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    SDriveEngine::inst()->createFolderDialog->exec();
}

void OperationsUI::slotAcceptCreateFolder(const QString& name)
{
    createFolder(name);
    delete SDriveEngine::inst()->createFolderDialog;
}

void OperationsUI::slotRejectCreateFolder()
{
    delete SDriveEngine::inst()->createFolderDialog;
}

void OperationsUI::slotFinishedCreateFolder(int result)
{
    delete SDriveEngine::inst()->createFolderDialog;
}

void OperationsUI::createFolder(const QString& name)
{
    TreeItemInfo item = *SDriveEngine::inst()->foldersManager->getParser()->getXMLHandler()->getTreeItemInfo();
    int itemIndex = SDriveEngine::inst()->foldersUI->getCurrentFolderItemIndex();

    if(name == "" || name.contains(QRegExp("[/\\\".<>]")))
    {
        CommonTools::msg("Please enter valid name");
        return;
    }

    SDriveEngine::inst()->foldersManager->createFolder(item[itemIndex].self, name);
    SDriveEngine::inst()->foldersManager->insertFolder(name, item[itemIndex].pointer);
}
