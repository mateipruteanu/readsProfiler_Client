#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <cstring>
#include <string>
#include <QDir>
#include <QTimer>

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

//using namespace std;

#define MAX_SIZE 100
#define MAX_FILE_SIZE 1024

int sd;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
		cout<<"Program Started\n\n";
		ui->stackedWidget->setCurrentIndex(2);
		// 0-> start
		// 1 -> main
		// 2 -> login & create
		// 3 -> create
    ui->download_button->hide();
    ui->title_input->text();
    setWindowTitle("libraryProject");
    //connectToServer();

}

MainWindow::~MainWindow() {
		sendMessage((char*)"close");
		clearBookInputFields();
		ui->stackedWidget->setCurrentIndex(1);
		disconnectFromServer();
		std::cout<<"disconnected"<<std::endl;
    delete ui;
}


void MainWindow::on_search_button_clicked()
{
		char answer[MAX_SIZE] = "";


    if(getTitleInput() == "" && getAuthorInput() == "" && getISBNInput() == "") {
        ui->download_button->hide();
				showStatusMessage((char*)"Please enter a title, author and/or ISBN to search!");
        std::cout << "Input Boxes Empty" << std::endl;
    }
    else {
        /// send them to server
				sendMessage((char*)"book_search");
				if(getTitleInput() == "" && getISBNInput() == "") {

            std::cout << "Title: " << "NULL" << std::endl;
						std::cout << "ISBN: " << "NULL" << std::endl;
						char message[MAX_SIZE] = "";

						strlcpy(message, "NULL", MAX_SIZE);
						sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);

						strlcpy(message, "NULL", MAX_SIZE);
						sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);

						strlcpy(message, "NULL", MAX_SIZE);
						sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);

				}
        else {
						// title
            std::cout << "Title: " << getTitleInput() << std::endl;
						char message[MAX_SIZE] = "";
						strlcpy(message, getTitleInput().c_str(), MAX_SIZE);
            sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);
            cout<<answer<<" "<<strlen(answer)<<endl;


						// author
						std::cout << "Author: " << getAuthorInput() << std::endl;
						strlcpy(message, getAuthorInput().c_str(), MAX_SIZE);
						sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);



						// ISBN
						std::cout << "ISBN: " << getISBNInput() << std::endl;
						strlcpy(message, getISBNInput().c_str(), MAX_SIZE);
						sendMessage(message);
						strlcpy(answer, receiveMessage(), MAX_SIZE);
        }

				strlcpy(answer, receiveMessage(), MAX_SIZE);

				if(!strcasecmp(answer, "book_found")) {
					/// if found, show download button and populate input fields

					strlcpy(answer, receiveMessage(), MAX_SIZE); // title
					cout<<answer<<" "<<strlen(answer)<<endl;
					ui->title_input->setText(answer);

					strlcpy(answer, receiveMessage(), MAX_SIZE); // author
					cout<<answer<<" "<<strlen(answer)<<endl;
					ui->author_input->setText(answer);

					strlcpy(answer, receiveMessage(), MAX_SIZE); // ISBN
					cout<<answer<<" "<<strlen(answer)<<endl;
					ui->ISBN_input->setText(answer);

					ui->download_button->show();
					showStatusMessage((char*)"Book found!");
				}
				else if(!strcasecmp(answer, "book_not_found")) {
					ui->download_button->hide();
					showStatusMessage((char*)"Book not found!");
				}

    }

}

string MainWindow::getTitleInput() {
    return ui->title_input->text().toStdString();
}

string MainWindow::getAuthorInput() {
    return ui->author_input->text().toStdString();
}

string MainWindow::getISBNInput() {
    return ui->ISBN_input->text().toStdString();
}



void MainWindow::on_login_button_clicked() {
		if(login()) {
				ui->stackedWidget->setCurrentIndex(1);
				ui->download_button->hide();
		}
		else {
				cout << "not logged in" << endl;
		}

}

bool MainWindow::login() {
		char username[MAX_SIZE] = "", password[MAX_SIZE] = "";
		strlcpy(username, ui->username_input->text().toStdString().c_str(), MAX_SIZE);
		strlcpy(password, ui->password_input->text().toStdString().c_str(), MAX_SIZE);
		username[strlen(username)] = '\0';
		//username[0] = 'X';
		sendMessage((char*)"login\0");
    cout<<"SENT_Login\n";
    sendMessage(username);
    cout<<receiveMessage()<<endl;
    cout<<"SENT_usr\n";

    sendMessage(password);
    cout<<receiveMessage()<<endl;
    cout<<"SENT_pwd\n";

		ui->username_input->setText("");
		ui->password_input->setText("");


    char answer[30] = "";
    strlcpy(answer, receiveMessage(), 30);
		cout<<"LOGIN_ANSWER: "<<answer<<endl;
    if(!strcasecmp(answer, "TRUE")) {
        cout<<"true\n";
        return true;
    }
    else {
        cout<<"false\n";
        return false;
    }
}



void MainWindow::sendMessage(char msg[]) {
    cout<<"waiting to send: "<<msg<<"\n";
		if(write (sd, msg, MAX_SIZE) <= 0) {
        perror("WRITE_ERROR");
    }
    cout<<"sent\n\n";
}

char* MainWindow::receiveMessage() {
		char *message = new char[MAX_SIZE];
		message[0] = '\0';
    cout<<"waiting to receive...\n";
		if(read (sd, message, MAX_SIZE) <= 0) {
        perror("READ_ERROR");
    }

		std::cout<< "[client]Mesajul primit este: " << message << std::endl;
		return message;
}



bool MainWindow::connectToServer() {
    struct sockaddr_in server;	// structura folosita pentru conectare


      /* stabilim portul */
      int port = 2728;

      /* cream socketul */
      if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
          perror ("[client] Eroare la socket().\n");
          return false;
        }


      /* umplem structura folosita pentru realizarea conexiunii cu serverul */
      /* familia socket-ului */
      server.sin_family = AF_INET;
      /* adresa IP a serverului */
			server.sin_addr.s_addr = inet_addr("0.0.0.0");
      /* portul de conectare */
      server.sin_port = htons (port);

      /* ne conectam la server */
      if (::connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
          perror ("[client]Eroare la connect().\n");
          return false;
        }
      //sendMessage("Hello");
      return true;
}


bool MainWindow::disconnectFromServer() {
		sendMessage((char*)"close");
		receiveMessage();
		::close (sd);
    return true;
}

void MainWindow::on_logout_button_clicked() {

		sendMessage((char*)"logout");
		clearBookInputFields();
		ui->stackedWidget->setCurrentIndex(2);
		cout<<receiveMessage();
		cout<<"[client]logged_out\n";
}

bool MainWindow::readData(int sock, void *buf, int buflen) {
	unsigned char *pbuf = (unsigned char *)buf;

	while (buflen > 0) {
		int num = recv(sock, pbuf, buflen, 0);
		if (num == 0)
			return false;
		pbuf += num;
		buflen -= num;
	}

	return true;
}

bool MainWindow::readFile(int sock, FILE *f) {
	long long filesize = 0;
	/// getting the size of the file from server
	if(!readData(sock, &filesize, sizeof(filesize))) {
		cout<<"filesize_error\n";
		return false;
	}

	filesize = ntohl(filesize);
	cout<<"FILESIZE = "<<filesize<<endl;

	if (filesize > 0) {
		printf("Receiving file of size %lld\n", filesize);
		char buffer[MAX_FILE_SIZE];
		do {
			long long MAX_RECEIVED_SIZE;
			if(filesize <= sizeof(buffer))
				MAX_RECEIVED_SIZE = filesize;
			else
				MAX_RECEIVED_SIZE = sizeof(buffer);

			if (!readData(sock, buffer, MAX_RECEIVED_SIZE))
				{
					return false;
				}
			int offset = 0;
			do {
				/// writing into the file until we write current buffer
				int written = fwrite(&buffer[offset], 1, MAX_RECEIVED_SIZE - offset, f);
				if (written < 1)
					return false;
				offset += written;
			} while (offset < MAX_RECEIVED_SIZE);
			filesize -= MAX_RECEIVED_SIZE;
			//cout<<"curr_FS = "<<filesize<<endl;
		} while (filesize > 0);
		printf("File received!\n");
		return true;
	}
	else
		return false;
}

void MainWindow::showStatusMessage(char msg[]) {
	ui->status_label->show();
	ui->status_label->setText(msg);
	QTimer::singleShot(2000, ui->status_label, &QWidget::hide);
}


void MainWindow::on_download_button_clicked() {
	cout<<"\n\n";
	cout<<QDir::currentPath().toStdString()<<endl;
	cout<<"\n\n";
	char fileName[MAX_SIZE] = "";
	sendMessage((char*)"book_download");
	cout<<"before receiving"<<endl;
	strlcpy(fileName, receiveMessage(), MAX_SIZE);
	cout<<"FILENAME: "<<fileName<<endl;

	FILE *downloadedFile = fopen(fileName, "wb+");

	if(readFile(sd, downloadedFile))
		cout<<"FILE DOWNLOADED\n";
	else
		cout<<"FILE NOT DOWNLOADED\n";

	showStatusMessage((char*)"File downloaded successfully!");
	ui->download_button->hide();
	clearBookInputFields();

}

void MainWindow::clearBookInputFields() {
	ui->title_input->setText("");
	ui->author_input->setText("");
	ui->ISBN_input->setText("");
}

void MainWindow::clearAllInputFields() {
	clearBookInputFields();
	ui->password_input->setText("");
	ui->username_input->setText("");
	ui->create_password1->setText("");
	ui->create_password2->setText("");
	ui->create_username->setText("");
}


void MainWindow::on_lucky_button_clicked()
{
		sendMessage((char*)"book_recommend");
		char msg[MAX_SIZE] = "";
		strlcpy(msg, receiveMessage(), MAX_SIZE); // received
		cout<<"FEELING LUCKY: "<<msg<<endl;
		strlcpy(msg, receiveMessage(), MAX_SIZE); // title
		ui->title_input->setText(msg);
		strlcpy(msg, receiveMessage(), MAX_SIZE); // author
		ui->author_input->setText(msg);
		strlcpy(msg, receiveMessage(), MAX_SIZE); // ISBN
		ui->ISBN_input->setText(msg);
}


void MainWindow::on_signin_button_clicked()
{
		ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_signup_button_clicked()
{
	ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_create_button_clicked() {
	char msg[MAX_SIZE] = "";
		cout<<"Create account!\n";
		if(!strcmp((char*)ui->create_password1->text().toStdString().c_str(), (char*)ui->create_password2->text().toStdString().c_str()) &&
			 strcmp((char*)ui->create_username->text().toStdString().c_str(), "") &&
			 strcmp((char*)ui->create_password1->text().toStdString().c_str(), "") &&
			 strcmp((char*)ui->create_password2->text().toStdString().c_str(), "")) {

			sendMessage((char*)"create_account");

			sendMessage((char*)ui->create_username->text().toStdString().c_str());
			sendMessage((char*)ui->create_password1->text().toStdString().c_str());

			strlcpy(msg, receiveMessage(), MAX_SIZE);
			if(!strcmp(msg, "created_account")) {
				showStatusMessage((char*)"Account created successfuly!");
				ui->stackedWidget->setCurrentIndex(0);
			}
			else if(!strcmp(msg, "account_already_exists")) {
				cout<<"already exists\n";
				showStatusMessage((char*)"This account already exists!");
				cout<<"SHOWED\n";
			}
			else
				showStatusMessage((char*)"Something went wrong while creating the account!");
		}
		else {
			cout<<"WRONG\n";
			showStatusMessage((char*)"Passwords don't match / You have empty input fields");
			cout<<"WRONG1\n";
		}
		clearAllInputFields();
}


void MainWindow::on_back_button_clicked()
{
		clearAllInputFields();
		ui->stackedWidget->setCurrentIndex(2);
}

