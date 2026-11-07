#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow()
{
    setWindowTitle("MirzArchive");
    resize(1000, 650);

    setupMenu();
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

    statusBar()->showMessage(QString("Yo! You picked this archive %1").arg(filePath));
    });

    //Seperator
    fileMenu->addSeparator();

    //Exit
    QAction* exitAction = new QAction("Exit");
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered,
        qApp, &QApplication::quit);
}