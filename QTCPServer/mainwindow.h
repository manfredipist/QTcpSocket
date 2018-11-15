#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMessage(QString);
private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);

    void readSocket();
    void discardSocket();

    void displayMessage(const QString& str);
    void sendMessage(QTcpSocket* socket);

    void on_pushButton_sendMessage_clicked();

private:
    Ui::MainWindow *ui;

    QTcpServer* m_server;
    QList<QTcpSocket*> connection_list;
};

#endif // MAINWINDOW_H
