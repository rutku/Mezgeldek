#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dropEvent(QDropEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);

private:
    QComboBox *cmbNewResolution;
    QLabel *lblResolution;
    QLabel *lblDragAndDrop;
    QProgressBar *progressBar;

    QMenu *fileMenu;
    QMenu *aboutMenu;
    QMenu *languageMenu;

    QAction *quitAction;
    QAction *applicationAbout;
    QAction *qtAbout;

    int imageWidth;
    int imageHeight;
    QString filesResizeDirectoryName;

    void createWidgets();
    void createLayouts();
    void createMenus();
    void createActions();
    void createConnections();
    void addResolutionsCmb();
    bool setWidthAndHeight(QString text);
    void directoryCreate(QList<QUrl> path);
    void imageResize(QString path);
    void aboutApplication();



};

#endif // MAINWINDOW_H
