#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer();

    if(m_server->listen(QHostAddress::Any, 8080))
    {
       connect(this,SIGNAL(newMessage(QString)),this,SLOT(displayMessage(QString)));
       connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

       this->ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    foreach (QTcpSocket* socket, connection_list)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket* socket)
{
    connection_list.append(socket);
    connect(socket, SIGNAL(readyRead()), this , SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this , SLOT(discardSocket()));
    this->ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
    displayMessages(QString("INFO::Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()));
}

void MainWindow::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray block = socket->readAll();
    QDataStream in(&block, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_12);

    while (!in.atEnd())
    {
        QString receiveString;
        in >> receiveString;
        receiveString.prepend(QString("%1 :: ").arg(socket->socketDescriptor()));
        emit newMessage(receiveString);
    }
}

void MainWindow::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    for (int i=0;i<connection_list.size();i++)
    {
        if (connection_list.at(i) == socket)
        {
            connection_list.removeAt(i);
            break;
        }
    }

    socket->deleteLater();
}

void MainWindow::on_pushButton_sendMessage_clicked()
{
    QString receiver = this->ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_list)
        {
            sendMessage(socket);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,connection_list)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendMessage(socket);
                break;
            }
        }
    }
    this->ui->lineEdit_message->clear();
}


void MainWindow::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = this->ui->lineEdit_message->text();

            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            out.setVersion(QDataStream::Qt_5_12);
            out << str;
            socket->write(block);
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::displayMessage(const QString& str)
{
    this->ui->textBrowser_receivedMessages->append(str);
}
