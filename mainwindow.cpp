#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myPlayer = new Player();
    QObject::connect(myPlayer, SIGNAL(sigCaptureImage(QImage)),
                                  this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(myPlayer, SIGNAL(sigDetectionImage(QImage)),
                                  this, SLOT(updatePlayerUI2(QImage)));
    QObject::connect(myPlayer, SIGNAL(sigDetectionResult(const char*)),
                                  this, SLOT(updateStatusBar(const char*)));
    ui->setupUi(this);
}

void MainWindow::updateStatusBar(const char* msg)
{
    statusBar()->showMessage(msg, 2000);
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::updatePlayerUI2(QImage img)
{
    if (!img.isNull())
    {
        ui->label_2->setAlignment(Qt::AlignCenter);
        ui->label_2->setPixmap(QPixmap::fromImage(img).scaled(ui->label_2->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

MainWindow::~MainWindow()
{
    delete myPlayer;
    delete ui;
}



void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_actionIniciar_Captura_triggered()
{
    if (myPlayer->isStopped())
    {
        myPlayer->play();
        ui->actionDetener_Captura->setDisabled(false);
        ui->actionIniciar_Captura->setDisabled(true);
        statusBar()->showMessage("Capturando...");
    }
}

void MainWindow::on_actionDetener_Captura_triggered()
{
    if (myPlayer->isRunning())
    {
        myPlayer->stop();
        ui->actionDetener_Captura->setDisabled(true);
        ui->actionIniciar_Captura->setDisabled(false);
        statusBar()->showMessage("Captura detenida",2000);
    }
}
