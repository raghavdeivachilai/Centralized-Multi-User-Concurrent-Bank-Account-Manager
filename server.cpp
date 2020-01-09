//Server Program to run concurrent clients
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <pthread.h>
typedef struct sockaddr_in SA_IN;

using namespace std;
map<int, pair<string, int>> account_details;
char client_message[100];
char j[130];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//called for each client thread
void *accounts(void *arg)
{
    int actnum, timestamp, amt;
    char type;
    //Get the socket descriptor
    int newSocket = (long)arg;
    char msg[100];
    cout<<"Client connected"<<" "<<newSocket<<" "<<"Thread ID"<<" "<<pthread_self()<<endl;
    int n;
    bool acct_exists = false;
    bzero(msg, 100);
    while (n = read(newSocket, msg, 1024))
    {
        if (n < 0)
        {
            cout << "Error" << endl;
        }
        //separating account id, transaction to be performed and the amount from the received data
        sscanf(msg, "%d\t%d\t%c\t%d", &timestamp, &actnum, &type, &amt);
        //check whether id in the requested transaction exists in the map
        auto act = account_details.find(actnum);

        if (act != account_details.end())
        {
            pthread_mutex_lock(&lock);
            acct_exists = true;
            //Withdrawal Operation
            if (type == 'w')
            {

                if (act->second.second - amt >= 0)
                {
                    act->second.second = act->second.second - amt;
                    int a = act->second.second;
                    /* Write a response to the client */
                    write(newSocket, &a, sizeof(act->second.second));
                }
                else
                {
                    //Balance insufficient to perform the withdrawal
                    cout << "Insufficient Balance!" << endl;
                }
            }
            //Deposit Operation
            else if (type == 'd')
            {

                act->second.second = act->second.second + amt;
                int a = act->second.second;
                /* Write a response to the client */
                write(newSocket, &a, sizeof(act->second.second));
            }
            else
            {
                /*Write a response to the client in case of invalid operation */
                write(newSocket, "Inavlid Operation!", 100);
            }
            pthread_mutex_unlock(&lock);
        }
        else
        {
            /* Write a response to the client if account does not exist */
            write(newSocket, "Account does not exist!", 100);
        }
    }
    return 0;
}

int main()
{

    SA_IN server_addr, client_addr;
    int acc_num, acc_amt;
    string acc_name;
    ifstream sfiledesc;
    sfiledesc.open("accounts.txt");
    if (sfiledesc.fail())
    {
        cout << "File not found" << endl;
        exit(1);
    }
    while (!sfiledesc.eof())
    {
        sfiledesc >> acc_num >> acc_name >> acc_amt;
        account_details.emplace(acc_num, make_pair(acc_name, acc_amt));
    }
    int server_socket;
    /* Call to socket() function */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9019);
    server_address.sin_addr.s_addr = INADDR_ANY;
    /* Bind the host address */
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    /* Now the process will start listening for the clients*/
    listen(server_socket, 100);
    pthread_t sockthreads[100], t;
    int i = 0, newsocket;
    int addr_size = sizeof(SA_IN);
    /* Accept actual connection from multiple clients */
    while (newsocket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size))
    {

        cout << "Connection accepted" << endl;
        /*create a new thread for each client */
        pthread_create(&t, NULL, accounts, (void *)newsocket);
    }
    //close the connection socket
    close(newsocket);
    //close the server socket
    pthread_exit(NULL);
    return 0;
}