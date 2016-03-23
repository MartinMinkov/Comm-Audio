#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0);
    ~server();

private:
    Ui::server *ui;
};

#endif // SERVER_H
