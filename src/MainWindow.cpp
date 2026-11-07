#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>
#include <QListWidget>
#include <QFileInfo>

MainWindow::MainWindow()
{
    setWindowTitle("MirzArchive");
    resize(1000, 650);

    setupMenu();
    setupFileView();
}

void MainWindow::setupMenu()
{
    //Open Archive
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = new QAction("Open Archive");
    fileMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, [this]() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Archive", "", "MirzArchive Files (*.mar)");

    if (filePath.isEmpty()) {
        return;
    }

    fileList->clear();
    fileList->addItem(QFileInfo(filePath).fileName());
    });

    //Seperator
    fileMenu->addSeparator();

    //Exit
    QAction* exitAction = new QAction("Exit");
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered,
        qApp, &QApplication::quit);
}

void MainWindow::setupFileView()
{
    fileList = new QListWidget(this);
    setCentralWidget(fileList);
}
