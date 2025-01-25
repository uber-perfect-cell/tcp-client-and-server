#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
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
    while(true) {
        string message, lastLine, currentLine;
        time_t now = time(0);
        char* dt = ctime(&now);
        ofstream testFile("logs.txt", ios_base::app);
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
    string username;

    cout << "username please! ";
    cin >> username;
    cin.ignore();

    int serverSocket;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    ssize_t bytesRead;
    struct sockaddr_in serverAddress;
    socklen_t addressLength = sizeof(serverAddress);
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Connection failed!" << endl;
        return 1;
    }
	send(clientSocket, "Client Connected!", 2500, 0);
	
    thread receivingThread(receivingData, clientSocket);
    thread sendingThread(sendingData, username, clientSocket, serverSocket);
    
    receivingThread.join();
    sendingThread.join();

    return 0;
}
