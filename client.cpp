#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    this->setFixedSize(470,383);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &client::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &client::disconnected);
}

client::~client()
{
    if(socket->isOpen()) socket->close();
    delete socket;
    delete ui;
}

void client::closeEvent(QCloseEvent *event)
{
    if(socket) socket->disconnected();
    emit windowClosed();
    event->accept();
}

void client::on_lineEdit_returnPressed()
{
    QString sending = ui->lineEdit->text();
    if(socket)
        {
            if(socket->isOpen())
            {
                QDataStream socketStream(socket);
                socketStream.setVersion(QDataStream::Qt_5_15);
                QByteArray byteArray = sending.toUtf8();
                socketStream << byteArray;

                sending.prepend("Me: ");
                ui->textBrowser->append(sending);
                ui->lineEdit->clear();
            }
            else QMessageBox::critical(this, "TCP Messenger", "Socket is not opened!");
        }
    else QMessageBox::critical(this, "TCP Messenger", "Socket is not connected!");
}

void client::readyRead()
{
    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
        ui->textBrowser->append("Server: More data coming...");
        return;
    }

    QString message = buffer;
    ui->textBrowser->append(message);


}
void client::disconnected()
{
    ui->statusbar->showMessage("Disconnected!");
    socket->close();
}

void client::on_pushButton_clicked()
{
    ip.setAddress(ui->lineEdit_2->text());
    socket->connectToHost(ip,321);
    if(socket->waitForConnected()) ui->statusbar->showMessage("Connected to the Server");
    else
    {
        ui->statusbar->showMessage("Failed to connect. Please retry.");
        socket->close();
    }


}
