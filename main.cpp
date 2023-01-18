#include "mainwindow.h"

#include <iostream>

#include <QApplication>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFile>
#include "mainwindow.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
		QCoreApplication::setApplicationName(QStringLiteral("your title"));
		w.setWindowTitle("readsProfiler");
    w.connectToServer();
    w.show();

    return a.exec();
}
