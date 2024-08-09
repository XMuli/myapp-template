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

private:
    Ui::MainWin *ui;
};

#endif // MAINWIN_H
