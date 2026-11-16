#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <FileManager.h>
#include <QTreeWidget>
#include <QString>

class FileTreeItem : public QTreeWidgetItem
{
public:
    bool operator<(const QTreeWidgetItem& other) const override;
};

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    void setupMenu();
    void setupFileView();
    void setupConnections();
    bool validateArchive(const QString& filePath);

    FileManager fileManager;
    QTreeWidget* fileList;
    QString currentArchivePath;
    QLineEdit* navigationBar;
};

#endif