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
#include <QHeaderView>
#include <QToolBar>

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
    QToolBar* toolBar = addToolBar("Main Toolbar");
    toolBar->addAction(openAction);
    QAction* closeAction = new QAction("Close Archive");
    fileMenu->addAction(closeAction);
    toolBar->addAction(closeAction);
    QAction* exitAction = new QAction("Exit");
    fileMenu->addAction(exitAction);
    closeAction->setEnabled(false);

    //Open archive
    connect(openAction, &QAction::triggered, this, [this, closeAction]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open Archive", "", "MirzArchive Files (*.mar)");

        if (filePath.isEmpty()) {
            return;
        }
        else if (!validateArchive(filePath)) {
            return;
        }
        else {
            currentArchivePath = filePath;
            closeAction->setEnabled(true);
            fileList->clear();

            QFileInfo fileInfo(filePath);

            QTreeWidgetItem* item = new QTreeWidgetItem();

            item->setText(0, fileInfo.fileName());
            item->setText(1, QString::number(fileInfo.size()) + " bytes");
            item->setText(2, fileInfo.suffix().toUpper());
            item->setText(3, fileInfo.lastModified().toString("dd/MM/yyyy hh:mm"));

            fileList->addTopLevelItem(item);
        }
    });

    //Close archive
    connect(closeAction, &QAction::triggered, this, [this, closeAction]() {
        fileList->clear();
        currentArchivePath.clear();
        statusBar()->clearMessage();
        closeAction->setEnabled(false);
    });

    //Separator
    fileMenu->addSeparator();

    //Exit action
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

    fileList->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    fileList->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    fileList->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    fileList->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
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