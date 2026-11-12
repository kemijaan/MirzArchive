#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <FileManager.h>
#include <QTreeWidget>
#include <QString>

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