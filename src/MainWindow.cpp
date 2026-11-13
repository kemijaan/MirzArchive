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
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

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

    QAction* openFolderAction = new QAction("Open Folder");
    fileMenu->addAction(openFolderAction);

    QToolBar* toolBar = addToolBar("Main Toolbar");
    toolBar->addAction(openAction);

    QAction* upAction = new QAction("Up");
    toolBar->addAction(upAction);

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

    //Open folder
    connect(openFolderAction, &QAction::triggered, this, [this]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "Open Folder");

        if (folderPath.isEmpty()) {
            return;
        }
        
        navigationBar->setText(folderPath);

        std::filesystem::path directoryPath = folderPath.toStdWString();

        std::vector<std::filesystem::directory_entry> entries = fileManager.listDirectory(directoryPath);

        fileList->clear();

        for (const auto& entry : entries) {
            QTreeWidgetItem* item = new QTreeWidgetItem();

            QString name = QString::fromStdString(entry.path().filename().string());

            item->setText(0, name);

            if (entry.is_directory()) {
                item->setText(1, "<DIR>");
                item->setText(2, "Folder");
            }
            else {
                item->setText(1, QString::number(entry.file_size()) + " bytes");
                item->setText(2, QString::fromStdString(entry.path().extension().string()));
            }

            QFileInfo fileInfo(QString::fromStdWString(entry.path().wstring()));
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

    //Up action
    connect(upAction, &QAction::triggered, this, [this](){
        QString currentPath = navigationBar->text();

        if(currentPath.isEmpty()){
            return;
        }

        std::filesystem::path directoryPath = currentPath.toStdWString();
        std::filesystem::path parentPath = directoryPath.parent_path();

        if (parentPath.empty() || parentPath == directoryPath){
            return;
        }

        std::vector<std::filesystem::directory_entry> entries = fileManager.listDirectory(parentPath);

        fileList->clear();
        navigationBar->setText(QString::fromStdWString(parentPath.wstring()));

        for(const auto& entry : entries){
            QTreeWidgetItem* item = new QTreeWidgetItem();

            QString name = QString::fromStdWString(entry.path().filename().wstring());
            item->setText(0, name);

            if(entry.is_directory()){
                item->setText(1, "<DIR>");
                item->setText(2, "Folder");
            }

            else{
                item->setText(1, QString::number(entry.file_size()) + " bytes");
                item->setText(2, QString::fromStdWString(entry.path().extension().wstring()));
            }
            
            QFileInfo fileInfo(QString::fromStdWString(entry.path().wstring()));
            item->setText(3, fileInfo.lastModified().toString("dd/MM/yyyy hh:mm"));
            
            fileList->addTopLevelItem(item);
        }
    });
}

void MainWindow::setupFileView()
{
    fileList = new QTreeWidget(this);

    fileList->setColumnCount(4);
    fileList->setHeaderLabels({"Name", "Size", "Type", "Date Modified"});

    navigationBar = new QLineEdit(this);
    navigationBar->setPlaceholderText("Current archive location...");

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    layout->addWidget(navigationBar);
    layout->addWidget(fileList);

    setCentralWidget(centralWidget);

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
    connect(fileList, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item, int column) {
        QString currentPath = navigationBar->text();
        QString selectedName = item->text(0);

        std::filesystem::path fullPath = std::filesystem::path(currentPath.toStdWString()) / selectedName.toStdWString();

        if (!std::filesystem::is_directory(fullPath)) {
            return;
        }

        std::vector<std::filesystem::directory_entry> entries = fileManager.listDirectory(fullPath);

        fileList->clear();
        navigationBar->setText(QString::fromStdWString(fullPath.wstring()));

        for (const auto& entry : entries){
            QTreeWidgetItem* newItem = new QTreeWidgetItem();

            QString name = QString::fromStdWString(entry.path().filename().wstring());
            newItem->setText(0, name);

            if(entry.is_directory()){
                newItem->setText(1, "<DIR>");
                newItem->setText(2, "Folder");
            }

            else{
                newItem->setText(1, QString::number(entry.file_size()) + " bytes");
                newItem->setText(2, QString::fromStdWString(entry.path().extension().wstring()));
            }

            QFileInfo fileInfo(QString::fromStdWString(entry.path().wstring()));
            newItem->setText(3, fileInfo.lastModified().toString("dd/MM/yyyy hh:mm"));
            
            fileList->addTopLevelItem(newItem);
        }

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