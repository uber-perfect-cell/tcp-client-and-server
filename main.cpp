#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
using namespace std;

void receivingData(int clientSocket) {
    char receivedData[1024] = { 0 }; 
    while(true) {
        memset(receivedData, 0, sizeof(receivedData));
        ssize_t bytesRead = recv(clientSocket, receivedData, sizeof(receivedData) - 1, 0);
        if (bytesRead > 0) {
            cout << receivedData << "\n";
        }
    }
}

void sendingData(const string &username, int clientSocket, int serverSocket) {
	string message, lastLine, currentLine;
	while(true) {
    	time_t now = time(0);
		char* dt = ctime(&now);
        ofstream testFile("kys.txt", ios_base::app);
        getline(cin, message);
        testFile << dt << "["<<username<<"] " <<  message << "\n";
        testFile.close();

        ifstream testFileRead("kys.txt");
        while (getline(testFileRead, currentLine)) {
            lastLine = currentLine;
        }
        testFileRead.close();

        cout << lastLine << "\n";
        send(clientSocket, lastLine.c_str(), lastLine.size(), 0);
	}
}


int main() {
	int clientSocket;
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	ssize_t bytesRead;
	struct sockaddr_in serverAddress;
	socklen_t addressLength = sizeof(serverAddress);
	char receivedData[1024] = { 0 };

    string message, lastLine, currentLine, username;
	cout << "username please! ";
	cin >> username;
	cin.ignore();

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
	    cerr << "Binding failed!" << endl;
	    return 1;
	}
	
	if (listen(serverSocket, 5) < 0) {
	    cerr << "Listening failed!" << endl;
	    return 1;
	}
	clientSocket = accept(serverSocket, (struct sockaddr*)&serverAddress, &addressLength);
	thread receivingThread(receivingData, clientSocket);
	thread sendingThread(sendingData, username, clientSocket, serverSocket);
	    
	receivingThread.join();
	sendingThread.join();
	
}
