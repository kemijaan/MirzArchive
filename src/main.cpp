#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("MirzArchive");
    window.resize(1000, 650);

    QMenu* fileMenu;
    QAction* openAction;
    QAction* exitAction;

    fileMenu = window.menuBar()->addMenu("File");

    openAction = new QAction("Open Archive");
    fileMenu->addAction(openAction);

    fileMenu->addSeparator();

    exitAction = new QAction("Exit");
    fileMenu->addAction(exitAction);

    QObject::connect(exitAction, &QAction::triggered,
                     &app, &QApplication::quit);

    QObject::connect(openAction, &QAction::triggered,
                     &window, [&window]() {

        QString filePath = QFileDialog::getOpenFileName(
            &window,
            "Open Archive",
            "",
            "MirzArchive Files (*.mar)"
        );

        if (filePath.isEmpty()) {
            return;
        }

    });

    window.show();

    return app.exec();
}