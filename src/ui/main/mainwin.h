#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();

private slots:
    void on_pushButton_released();

private:
    void initUI();

private:
    Ui::MainWin *ui;
};

#endif // MAINWIN_H
