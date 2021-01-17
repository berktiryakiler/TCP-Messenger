#ifndef WELCOME_H
#define WELCOME_H

#include <QDialog>
#include <QGuiApplication>
#include "server.h"
#include "client.h"

namespace Ui {
class welcome;
}

class welcome : public QDialog
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = nullptr);
    ~welcome();
    server *svr = nullptr;
    client *clt = nullptr;
signals:


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void appearWindow();

private:
    Ui::welcome *ui;
};

#endif // WELCOME_H
