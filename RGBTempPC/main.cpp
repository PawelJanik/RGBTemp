#include "rgbtemppc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RGBTempPC w;
	w.show();

	return a.exec();
}
