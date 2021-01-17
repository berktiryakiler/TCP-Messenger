#include "welcome.h"
#include "ui_welcome.h"

welcome::welcome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
    this->setFixedSize(400,150);

}

welcome::~welcome()
{
    delete ui;

}

void welcome::appearWindow()
{
    this->show();

    if(clt != nullptr)
    {
        delete clt;
        clt = nullptr;
    }

    if(svr != nullptr)
    {
        delete svr;
        svr = nullptr;
    }


}

void welcome::on_pushButton_clicked()
{   
    svr = new server();
    svr->setWindowTitle("Server");
    svr->show();
    connect(svr, SIGNAL(windowClosed()), this, SLOT(appearWindow()));

    this->hide();

}

void welcome::on_pushButton_2_clicked()
{
    clt = new client();
    clt->setWindowTitle("Client");
    clt->show();
    connect(clt, SIGNAL(windowClosed()), this, SLOT(appearWindow()));

    this->hide();
}
