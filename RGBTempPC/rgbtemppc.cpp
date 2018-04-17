#include "rgbtemppc.h"
#include "ui_rgbtemppc.h"

#include <iostream>

RGBTempPC::RGBTempPC(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RGBTempPC)
{
	ui->setupUi(this);

	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectPort()));
	connect(ui->temperatureInsideButton, SIGNAL(clicked()), this, SLOT(changeMode()));
	connect(ui->temperatureOutsideButton, SIGNAL(clicked()), this, SLOT(changeMode()));
	connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(changeMode()));
	connect(ui->lightButton, SIGNAL(clicked()), this, SLOT(changeMode()));
	connect(ui->colorSlider, SIGNAL(valueChanged(int)), this, SLOT(changeMode()));
	connect(ui->brightnessCheckBox, SIGNAL(clicked()), this, SLOT(changeBrihtness()));
	connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(changeBrihtness()));


	connect(timer, SIGNAL(timeout()), this, SLOT(getData()));

	QList<QSerialPortInfo> portList = portInfo.availablePorts();

	for(int i = 0; i < portList.count(); i++)
	ui->portNamecomboBox->addItem(portList.at(i).portName());
}

RGBTempPC::~RGBTempPC()
{
	disconnectPort();

	delete ui;
}

void RGBTempPC::connectPort()
{
	port.setBaudRate(QSerialPort::Baud19200);
	port.setDataBits(QSerialPort::Data8);
	port.setParity(QSerialPort::NoParity);
	port.setStopBits(QSerialPort::OneStop);
	port.setPortName(ui->portNamecomboBox->currentText());

	if(port.open(QIODevice::ReadWrite))
	{
		ui->statusBar->showMessage("Port is connect");	ui->connectButton->setText("Disconnect");
		ui->portNamecomboBox->setEnabled(false);
		disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectPort()));
		connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectPort()));

		port.write("D");
		timer->start(2000);
	}
	else
		ui->statusBar->showMessage("Kulfa mać");


}

void RGBTempPC::disconnectPort()
{
	if(port.isOpen())
	{

		port.close();
		ui->connectButton->setText("connect");
		ui->statusBar->showMessage("Port is disconnect");
		ui->portNamecomboBox->setEnabled(true);
		connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectPort()));
		disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectPort()));

		timer->stop();
	}
}

void RGBTempPC::getData()
{
	port.waitForReadyRead(200);

	char buffer[13];
	port.read(buffer,13);

	std::cout << "Data:\t" << buffer << "\n";

	if(buffer[0] == 'H' && buffer[3] == 'T' && buffer[4] == '1' && buffer[7] == 'T' && buffer[8] == '2')
	{
		QString qBuffer(buffer);

		QString temperatureOut;
		if(qBuffer.at(9) == '-')
			temperatureOut.append(qBuffer[9]);
		temperatureOut.append(qBuffer[10]);
		temperatureOut.append(qBuffer[11]);
		temperatureOut.append(" *C");
		ui->temperatureOutsideLabel->setText(temperatureOut);

		QString temperatureIn;
		temperatureIn.append(qBuffer[5]);
		temperatureIn.append(qBuffer[6]);
		temperatureIn.append(" *C");
		ui->temperatureInsideLabel->setText(temperatureIn);

		QString humidity;
		humidity.append(qBuffer[1]);
		humidity.append(qBuffer[2]);
		humidity.append(" %");
		ui->humidityLabel->setText(humidity);
	}
	port.clear();
	port.write("D");
}


void RGBTempPC::changeMode()
{
	timer->stop();
	if(ui->temperatureInsideButton->isChecked() == true)
	{
		port.write("T1\n");
		std::cout << "T1" << "\n";
		ui->colorSlider->setEnabled(false);
		ui->brightnessSlider->setEnabled(false);
		ui->brightnessCheckBox->setEnabled(false);
	}
	else if(ui->temperatureOutsideButton->isChecked() == true)
	{
		port.write("T2\n");
		std::cout << "T2" << "\n";
		ui->colorSlider->setEnabled(false);
		ui->brightnessSlider->setEnabled(false);
		ui->brightnessCheckBox->setEnabled(false);
	}
	else if(ui->colorButton->isChecked() == true)
	{
		ui->colorSlider->setEnabled(true);
		ui->brightnessSlider->setEnabled(false);
		ui->brightnessCheckBox->setEnabled(false);
		changeColor();
	}
	else if(ui->lightButton->isChecked() == true)
	{
		ui->colorSlider->setEnabled(false);
		ui->brightnessSlider->setEnabled(true);
		ui->brightnessCheckBox->setEnabled(true);
		changeBrihtness();
	}
	timer->start(2000);
}

void RGBTempPC::changeColor()
{
	QByteArray color = "C";
	int num =ui->colorSlider->value();

	if(num < 10)
		color.append("000");
	else if(num < 100)
		color.append("00");
	else if(num < 1000)
		color.append("0");

	color.append(QString::number(num));
	color.append("\n");
	writeData(color);

	std::cout << color.toStdString() << "\n";
}

void RGBTempPC::changeBrihtness()
{
	if(ui->brightnessCheckBox->isChecked() == true)
	{
		writeData("LA\n");
		ui->brightnessSlider->setEnabled(false);
	}
	else
	{
		ui->brightnessSlider->setEnabled(true);
		QByteArray color = "L";
		int num = 255 - ui->brightnessSlider->value();

		if(num < 10)
			color.append("00");
		else if(num < 100)
			color.append("0");

		color.append(QString::number(num));
		color.append("\n");
		writeData(color);

		std::cout << color.toStdString() << "\n";
	}
}

void RGBTempPC::writeData(const QByteArray &data)
{
	port.write(data);
}
