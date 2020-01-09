/*Client program to send multiple transactions */
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <string.h>

using namespace std;
int main()
{
    //open the filename
    fstream cfiledes("transactions.txt");
    if (cfiledes.fail())
    {
        cout << "File not found" << endl;
        exit(1);
    }
    int client_socket, connection_status;
    char item[100];
    string item1;
    int server_response;
    int initial = 0;
    int timestp;
    //create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9019);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //connect to server socket
    connection_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        printf("Connection Failed!");
    }
    //get each transaction and send it one by one
    while (getline(cfiledes, item1))
    {

        size_t t;
        t = item1.copy(item, item1.length(), 0);
        item[t + 1] = '\0';
        strcpy(item, item1.c_str());
        int sendres = write(client_socket, item, sizeof(item));
        read(client_socket, &server_response, sizeof(server_response));
        //read the acknowledgement received from server
        cout <<"Successful.The resulting balance is"<<" "<<server_response << endl;
        //sleep(0.5);
    }
    //close the client socket
    close(client_socket);

    return 0;
}
