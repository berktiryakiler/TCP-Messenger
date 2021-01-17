#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    this->setFixedSize(470,383);

    mServer = new QTcpServer(this);
    QString localhostname = QHostInfo::localHostName();
    QString localhostIP;
    QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    foreach(const QHostAddress& address, hostList)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) localhostIP = address.toString();
    }

    QHostAddress ip(localhostIP);
    if(mServer->listen(ip, 321))
    {
        connect(mServer, SIGNAL(newConnection()), this, SLOT(newConnection()), Qt::DirectConnection);
        ui->statusbar->showMessage("Listening... Your IP Address: " + localhostIP);
    }
    else
    {
        ui->statusbar->showMessage("Failed to connect!");
    }
}

server::~server()
{
    foreach(QTcpSocket* socket, socketList)
        {
            socket->close();
            socket->deleteLater();
        }

        mServer->close();
        mServer->deleteLater();

    delete ui;
}

void server::closeEvent(QCloseEvent *event)
{
    foreach(QTcpSocket* socket, socketList)
        {
            socket->close();
            socket->deleteLater();
        }
    mServer->close();
    mServer->deleteLater();
    emit windowClosed();
    event->accept();
}

void server::newConnection()
{
    socket = mServer->nextPendingConnection();
    socketList.push_back(socket);
    QString str = "---Welcome to the server!---";

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    QByteArray buffer = str.toUtf8();

    socketStream << buffer;

    connect(socket, &QTcpSocket::readyRead, this, &server::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &server::disconnected);

    ui->textBrowser->append("-> " + socket->peerAddress().toString() + " has entered the Server!");

    QString sIP = socket->peerAddress().toString();
    QString message = buffer;
    message.prepend(sIP + ": ");
    foreach(QTcpSocket* socket, socketList)
    {
        if(socket)
            {
                QString temp_sIP = socket->peerAddress().toString();
                if(socket->isOpen() && temp_sIP!=sIP)
                {
                    QDataStream socketStream(socket);
                    socketStream.setVersion(QDataStream::Qt_5_15);
                    QByteArray byteArray = message.toUtf8();

                    socketStream << byteArray;
                }
             }
    }

}

void server::readyRead()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        ui->textBrowser->append("->More data coming...");
        return;
    }

    QString message = buffer;
    QString sIP = socket->peerAddress().toString();
    message.prepend(sIP + ": ");
    ui->textBrowser->append(message);

    foreach(QTcpSocket* socket, socketList)
    {
        if(socket)
            {
                QString temp_sIP = socket->peerAddress().toString();
                if(socket->isOpen() && temp_sIP!=sIP)
                {
                    QDataStream socketStream(socket);
                    socketStream.setVersion(QDataStream::Qt_5_15);
                    QByteArray byteArray = message.toUtf8();

                    socketStream << byteArray;
                }
             }
    }

    socket = nullptr;
    delete socket;
}

void server::sendMessage(QString &sending)
{
    QString socketIP;
    sending.prepend("Server: ");

    foreach(QTcpSocket* socket, socketList)
    {
        socketIP = socket->peerAddress().toString();

        if(socket)
            {
                if(socket->isOpen())
                {
                    QDataStream socketStream(socket);
                    socketStream.setVersion(QDataStream::Qt_5_15); 
                    QByteArray byteArray = sending.toUtf8();

                    socketStream << byteArray;
                }

                else ui->textBrowser->append("->" + socketIP + "'s Socket is not opened!");
             } 
        else ui->textBrowser->append("->" + socketIP + "'s Socket is not connected!");
    }
    if(socketList.empty()) QMessageBox::critical(this, "TCP Messenger", "There is no socket to send message!");

    ui->textBrowser->append(sending);
    ui->lineEdit->clear();
}

void server::on_lineEdit_returnPressed()
{
    QString sending = ui->lineEdit->text();
    sendMessage(sending);
}

void server::disconnected()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    ui->textBrowser->append("-> " + socket->peerAddress().toString() + " has left!");
    socketList.removeOne(socket);

    socket->deleteLater();
}
