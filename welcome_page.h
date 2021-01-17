#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include <QDialog>

namespace Ui {
class welcome_page;
}

class welcome_page : public QDialog
{
    Q_OBJECT

public:
    explicit welcome_page(QWidget *parent = nullptr);
    ~welcome_page();

private:
    Ui::welcome_page *ui;
};

#endif // WELCOME_PAGE_H
