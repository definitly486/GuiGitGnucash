#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Этот макрос необходим для использования сигналов и слотов

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     void appendMessage(const QString &message); // объявляем функцию

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_cursorPositionChanged();



    void on_textEdit_undoAvailable(bool b);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
