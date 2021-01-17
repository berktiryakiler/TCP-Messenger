#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();
    void closeEvent(QCloseEvent *event);
signals:
    void windowClosed();
private slots:
    void on_lineEdit_returnPressed();
    void readyRead();
    void disconnected();

    void on_pushButton_clicked();

private:
    Ui::client *ui;
    QTcpSocket* socket;
    QHostAddress ip;
};

#endif // CLIENT_H
