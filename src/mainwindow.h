

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class DataBinding;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



public slots:
    void generate();

protected:
    void closeEvent(QCloseEvent *event);

private:

    // Sets up the data bindings between the UI and app settings
    void dataBind();



    QList<DataBinding*> m_bindings;

    Ui::MainWindow *m_ui;
    GLWidget *m_glWidget;


};

#endif // MAINWINDOW_H

