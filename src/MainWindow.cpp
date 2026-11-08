#include "MainWindow.h"
#include "ArchiveValidator.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QStatusBar>
#include <QFileInfo>
#include <QTreeWidget>

MainWindow::MainWindow()
{
    setWindowTitle("MirzArchive");
    resize(1000, 650);

    setupMenu();
    setupFileView();
    setupConnections();
}

void MainWindow::setupMenu()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = new QAction("Open Archive");
    fileMenu->addAction(openAction);

    connect(openAction, &QAction::triggered, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open Archive", "", "MirzArchive Files (*.mar)");

        if (filePath.isEmpty()) {
            return;
        }
        else if (!validateArchive(filePath)) {
            return;
        }
        else {
            currentArchivePath = filePath;
            fileList->clear();

            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, QFileInfo(filePath).fileName());
            fileList->addTopLevelItem(item);
        }
    });

    fileMenu->addSeparator();

    QAction* exitAction = new QAction("Exit");
    fileMenu->addAction(exitAction);

    connect(exitAction, &QAction::triggered, this, []() {
    QApplication::quit();
    });
}

void MainWindow::setupFileView()
{
    fileList = new QTreeWidget(this);

    fileList->setColumnCount(4);
    fileList->setHeaderLabels({"Name", "Size", "Type", "Date Modified"});

    setCentralWidget(fileList);
}

void MainWindow::setupConnections()
{
    connect(fileList, &QTreeWidget::itemClicked, this, [this]() {
        statusBar()->showMessage(currentArchivePath);
    });
}

bool MainWindow::validateArchive(const QString& filePath)
{
    ArchiveValidator validator;

    if (!validator.isValid(filePath.toStdString())) {
        statusBar()->showMessage("Invalid MirzArchive file!");
        return false;
    }
    else {
        statusBar()->showMessage("Valid MirzArchive file!");
        return true;
    }
}