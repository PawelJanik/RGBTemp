#ifndef RGBTEMPPC_H
#define RGBTEMPPC_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

namespace Ui {
class RGBTempPC;
}

class RGBTempPC : public QMainWindow
{
	Q_OBJECT

public:
	explicit RGBTempPC(QWidget *parent = 0);
	~RGBTempPC();

private:
	Ui::RGBTempPC *ui;

	QSerialPortInfo portInfo;
	QSerialPort port;

	QTimer *timer = new QTimer(this);

public slots:
	void connectPort();
	void disconnectPort();
	void getData();
	void changeMode();
	void changeColor();
	void changeBrihtness();
	void writeData(const QByteArray &data);
};

#endif // RGBTEMPPC_H
