#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include "MenuInterface.h"
class QTextEdit;
class QLineEdit;

namespace Ui {
class MyClient;
}

class MyClient : public MenuWidgetInterface
{
    Q_OBJECT
private:
    QTcpSocket *m_Socket;
    QByteArray Data;
    quint16 m_nextBlockSize;
    QByteArray data;
    QTimer *timer;
public:
    MyClient();
    ~MyClient();
private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();
    void slotDisconnected();
    void slotTimerAlarm();

    void on_connectButton_clicked();

    void on_endlessbtn_clicked();

private:
    Ui::MyClient *ui;

    // MenuWidgetInterface interface
public:
    QMenu *getMenu();
};

#endif // MYCLIENT_H
