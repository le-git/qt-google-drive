#ifndef DEF_H
#define DEF_H

#define ROOT_FOLDER "https://docs.google.com/feeds/default/private/full/folder%3Aroot"
#define PARENT_FOLDER "http://schemas.google.com/docs/2007#parent"
#define HIERARCHY_ATTRIBUTE attribs.value("rel")
#define SELF_TAG "self"
#define HIERARCHY_VALUE attribs.value("href")
#define TITLE_TAG "title"
#define GET_FOLDERS QString("https://docs.google.com/feeds/default/private/full/-/folder?showroot=true")
#define GET_FILES QString("https://docs.google.com/feeds/default/private/full")
#define FOLDER_TYPE 0
#define FILE_TYPE 1
#define UNKNOWN_TYPE_STR "unknown"
#define FILE_TYPE_STR "file"
#define FOLDER_TYPE_STR "folder"
#define TREE_VIEW_MAIN_TITLE "My Disc"
#define INFO_TOKEN '|'
#define SRC "src"
#define CONTENT "content"
#define WORK_DIR QString("WorkDir")

#endif // DEF_H
