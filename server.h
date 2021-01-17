#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QCloseEvent>
#include <QNetworkInterface>
#include <QHostInfo>

namespace Ui {
class server;
}

class server : public QMainWindow
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();
    void closeEvent(QCloseEvent *event);
    QTcpSocket *socket;
signals:
    void windowClosed();
public slots:
    void readyRead();
    void disconnected();
    void newConnection();
    void sendMessage(QString &sending);

private:
    Ui::server *ui;
    QTcpServer *mServer;
    QList<QTcpSocket*> socketList;

protected:


private slots:
    void on_lineEdit_returnPressed();
};

#endif // SERVER_H
