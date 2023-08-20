#include "myclient.h"
#include "ui_myclient.h"
#include <QTcpSocket>
#include <QTime>
#include <QPushButton>

MyClient::MyClient() :
    ui(new Ui::MyClient)
{
    ui->setupUi(this);

    //connect(m_Socket, &QTcpSocket::connected, this, &MyClient::slotConnected);

    m_Socket = new QTcpSocket(this);
    connect(m_Socket, &QTcpSocket::connected, this, &MyClient::slotConnected);
    connect(m_Socket, &QTcpSocket::readyRead, this, &MyClient::slotReadyRead);
    connect(m_Socket, &QTcpSocket::disconnected, this, &MyClient::slotDisconnected);
    //connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)),
//    this, SLOT(slotError(QAbstractSocket::SocketError))
//    );
    ui->textEdit->setReadOnly(true);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MyClient::slotSendToServer);
    connect(ui->lineEdit, &QLineEdit::returnPressed,
            this, &MyClient::slotSendToServer);
    m_nextBlockSize = 0;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
}

MyClient::~MyClient()
{
    m_Socket->disconnectFromHost();
    m_Socket->deleteLater();
}

void MyClient::slotReadyRead()
{
    //m_Socket = (QTcpSocket*) sender();
    QDataStream in(m_Socket);
    in.setVersion(QDataStream::Qt_5_4);
    if(in.status() == QDataStream::Ok)
    {

        for (;;) {
            if(m_nextBlockSize == 0)
            {
                if(m_Socket->bytesAvailable()  < 2)
                {
                    break;
                }
                in >> m_nextBlockSize;
            }
            if(m_Socket->bytesAvailable()  < m_nextBlockSize)
            {
                break;
            }
            QTime time;
            QString str = "HTTP/1.1 200 OK \r\n\r\n%1";;
            in >> time >> str;
            m_nextBlockSize = 0;
            ui->textEdit->append(time.toString() + " " + str);
        }
    }
    else {
        ui->textEdit->append("streamErroe");
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error:" +
            (err == QAbstractSocket::HostNotFoundError ?
                 "The host was not found." :
                 err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." : QString(m_Socket->errorString()));
}

void MyClient::slotSendToServer()
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << quint16(0) << QTime::currentTime() << ui->lineEdit->text();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    m_Socket->write(data);
    ui->lineEdit->clear();
}

void MyClient::slotConnected()
{
    ui->textEdit->append("Received the connected() signal");
}

void MyClient::slotDisconnected()
{
    ui->textEdit->append("Disconnect");
    timer->stop();
    m_Socket->disconnectFromHost();
    m_Socket->deleteLater();
    m_Socket = nullptr;
}

void MyClient::slotTimerAlarm()
{
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << quint16(0) << QTime::currentTime();
    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16));
    m_Socket->write(data);
}

void MyClient::on_connectButton_clicked()
{
    if(m_Socket == nullptr)
    {
        m_Socket = new QTcpSocket;
        connect(m_Socket, &QTcpSocket::connected, this, &MyClient::slotConnected);
        connect(m_Socket, &QTcpSocket::readyRead, this, &MyClient::slotReadyRead);
        connect(m_Socket, &QTcpSocket::disconnected, this, &MyClient::slotDisconnected);
    }
    m_Socket->connectToHost("192.168.0.18", 2323);
}

void MyClient::on_endlessbtn_clicked()
{
    timer->start(1000); // И запустим таймер
}

QMenu *MyClient::getMenu()
{
    return NULL;
}
