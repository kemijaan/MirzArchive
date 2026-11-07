#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <FileManager.h>
#include <QListWidget>

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    void setupMenu();
    void setupFileView();
    FileManager fileManager;
    QListWidget* fileList;
};

#endif