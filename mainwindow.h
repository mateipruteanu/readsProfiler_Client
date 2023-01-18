#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int myFunction();
    string getTitleInput();
    string getAuthorInput();
    string getISBNInput();

		void clearBookInputFields();
		void clearAllInputFields();
    bool connectToServer();
    bool disconnectFromServer();
    void sendMessage(char msg[]);
    char *receiveMessage();
    bool login();
		bool readData(int sock, void *buf, int buflen);
		bool readFile(int sock, FILE *f);
		void showStatusMessage(char msg[]);


    int n = 2;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_search_button_clicked();
    void on_login_button_clicked();

		void on_logout_button_clicked();

		void on_download_button_clicked();

		void on_lucky_button_clicked();

		void on_signin_button_clicked();

		void on_signup_button_clicked();

		void on_create_button_clicked();

		void on_back_button_clicked();

	private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
