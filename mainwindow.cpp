#include "mainwindow.h"

#include <QFileInfo>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createWidgets();
    createLayouts();
    createMenus();
    createActions();
    createConnections();
    addResolutionsCmb();

    setWindowTitle("Mezgeldek");

}

MainWindow::~MainWindow()
{
    delete lblResolution;
    delete cmbNewResolution;
    delete lblDragAndDrop;
    delete progressBar;
}

void MainWindow::dropEvent(QDropEvent *e)
{
    lblDragAndDrop->setText("");
    const QMimeData* mimeData = e->mimeData();
        // check for our needed mime type, here a file or a list of files
        if (mimeData->hasUrls())
        {
            QList<QUrl> urlList = mimeData->urls();
            if (setWidthAndHeight(cmbNewResolution->currentText()))
                directoryCreate(urlList);
        }

}

void MainWindow::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();
}

void MainWindow::createWidgets()
{
    cmbNewResolution = new QComboBox(this);
    lblResolution = new QLabel(tr("Resolution : "),this);
    lblDragAndDrop = new QLabel(tr("Leave Right Here !"),this);
    progressBar = new QProgressBar(this);

}

void MainWindow::createLayouts()
{
    QWidget *widget = new QWidget(this);

    QHBoxLayout *hRLayout = new QHBoxLayout();
    hRLayout->addWidget(lblResolution);
    hRLayout->addWidget(cmbNewResolution);

    QGroupBox *groupBox = new QGroupBox(tr("Drag and Drop"),this);
    QHBoxLayout *hGLayout = new QHBoxLayout();
    hGLayout->addSpacerItem(new QSpacerItem(50,20));
    hGLayout->addWidget(lblDragAndDrop);
    groupBox->setLayout(hGLayout);

    QVBoxLayout *vLayout = new QVBoxLayout(widget);
    vLayout->addLayout(hRLayout);
    vLayout->addWidget(groupBox);
    vLayout->addWidget(progressBar);

    widget->setLayout(vLayout);
    setCentralWidget(widget);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    languageMenu = new QMenu(tr("&Languages"),this);
    fileMenu->addMenu(languageMenu);
    aboutMenu = menuBar()->addMenu(tr("&About"));
}

void MainWindow::createActions()
{
    quitAction = new QAction(tr("&Quit"),this);
    quitAction->setShortcut(QKeySequence::Quit);
    applicationAbout = new QAction(tr("About"));
    qtAbout = new QAction(tr("Qt About"));

    fileMenu->addAction(quitAction);
    aboutMenu->addAction(applicationAbout);
    aboutMenu->addAction(qtAbout);
}

void MainWindow::createConnections()
{
    connect(quitAction,&QAction::triggered,this,&MainWindow::close);
    connect(applicationAbout,&QAction::triggered,this,&MainWindow::aboutApplication);
    connect(qtAbout,&QAction::triggered,qApp,&QApplication::aboutQt);

}

void MainWindow::addResolutionsCmb()
{
    cmbNewResolution->setEditable(true);

    cmbNewResolution->addItem("1680x1050");
    cmbNewResolution->addItem("1400x1050");
    cmbNewResolution->addItem("1280x1024");
    cmbNewResolution->addItem("1280x960");
    cmbNewResolution->addItem("1152x864");
    cmbNewResolution->addItem("1024x768");
    cmbNewResolution->addItem("800x600");
    cmbNewResolution->addItem("640x480");
    cmbNewResolution->addItem("720x400");
    cmbNewResolution->addItem("640x400");
    cmbNewResolution->addItem("640x350");

}

bool MainWindow::setWidthAndHeight(QString text)
{
    QStringList wh = text.split("x");
    if(wh.size()!=2)
        return false;

    imageWidth = wh.at(0).toInt();
    imageHeight = wh.at(1).toInt();
    return true;

}

void MainWindow::directoryCreate(QList<QUrl> path)
{
    QStringList images;
    QStringList nameFilters;
    nameFilters.append("*.jpeg");
    nameFilters.append("*.jpg");
    nameFilters.append("*.png");

    foreach(QUrl url, path)
    {
        QFileInfo file(url.toLocalFile());
        if(file.isDir())
        {
            QDir dir(file.absoluteFilePath());
            QStringList imagesList = dir.entryList(nameFilters,QDir::Files);
            foreach(QString s,imagesList)
                images.append(file.absoluteFilePath()+QDir::separator()+s);
        }else
            images.append(file.absoluteFilePath());
    }

    progressBar->setMaximum(images.size());

    for(int i = 0;i<images.size();i++)
    {
        imageResize(images.at(i));
        progressBar->setValue(i);
        if((images.size()-i)==1)
            progressBar->setValue(i+1);
    }
    lblDragAndDrop->setText(tr("Done !"));

}

void MainWindow::imageResize(QString path)
{
    filesResizeDirectoryName="resized";
    QStringList pathSplit = path.split(".");
    QString extension = pathSplit.at(pathSplit.count()-1);
    QFileInfo file(path);
    QString newImageSavePath = file.absolutePath() + QDir::separator() + filesResizeDirectoryName;
    QFileInfo newPath(newImageSavePath);
    if(!newPath.isDir())
        newPath.dir().mkdir(newImageSavePath);

    QImage orginalImage(path);
    QImage resizeImage(orginalImage.scaled(imageWidth,imageHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    resizeImage.save(newImageSavePath+QDir::separator()+file.fileName(),extension.toLatin1());

}

void MainWindow::aboutApplication()
{

    QMessageBox::about(this,QString::fromUtf8("Mezgeldek About"),
                       QString::fromUtf8("<a href='http://gitorious.org/mezgeldek'>Mezgeldek 1.0</a>"
                                         "<br>This is image resizing application."
                                         "<br>This application is licensed with BSD.</br>"
                                         "<br>Contact : ramazanutku@gmail.com</br>"));

}
