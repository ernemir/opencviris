#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <player.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    //Display video frame in player UI
    void updatePlayerUI(QImage img);
    void updatePlayerUI2(QImage img);
    void updateStatusBar(const char *msg);

    void on_actionSalir_triggered();
    void on_actionIniciar_Captura_triggered();
    void on_actionDetener_Captura_triggered();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    Player *myPlayer;
};

#endif // MAINWINDOW_H
