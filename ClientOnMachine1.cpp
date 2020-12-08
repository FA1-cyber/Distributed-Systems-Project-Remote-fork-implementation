#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>                     /* add by Kun*/
#include <stdlib.h>                     /* add by Kun*/
#include <stdio.h>                      /* add by Kun*/
#include <unistd.h>  
#include <iostream>
#include <sys/types.h>
#include<unistd.h>
#define SERVER_PORT 12345               /* arbitrary, but client and server must agree */
#define BUF_SIZE 102040  
using namespace std;                 /* block transfer size */
void fatal(string ss)
{
  cout<< ss<<endl;
  exit(1);
}
int main(int argc, char **argv)
{
    int c, s, bytes;
    char buf[BUF_SIZE];                   /* buffer for incoming file */
    char filename[300];                    /* file name */
    struct hostent *h;                    /* info about server */
    struct sockaddr_in channel;           /* holds IP address */


    if (argc != 3){
          fatal("Usage: client server-name file-name\n");
    }
    strcpy(filename, argv[2]);
  
    
    if (!(h = gethostbyname(argv[1]))){
          fatal("gethostbyname failed\n");
    }else{
          cout<<"host address is %s\n"<< argv[1]<<endl;
    }
    
    
    //socket
    if ((s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
          fatal("socket build error\n");
    }else{
          cout<<"socket build successfully\n"<<endl;
    }

    memset(&channel, 0, sizeof(channel));
    channel.sin_family= AF_INET;
    memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
    //channel.sin_addr.s_addr = inet_addr("10.0.2.15");
    //InetPton(AF_INET, _T("10.0.2.15"),&channel.sin_addr.s_addr);
    channel.sin_port= htons(SERVER_PORT);
    //  cout<<h->h_name<<endl;
    //  cout<<h->h_addr_list<<endl;
    //  cout<<h->h_addrtype<<endl;
    //  cout<<h->h_aliases<<endl;
    //  cout<<h->h_length<<endl;
    //connect
    
    if ((c = connect(s, (struct sockaddr *) &channel, sizeof(channel))) < 0){
      
      fatal("Error while connecting to IP (check IP of server if correct)\n");

    }else{
          cout<<"connected to server\n"<<endl;
    }
  
    recv(s, buf, BUF_SIZE, 0); // Welcome to TCP server message
    cout<<"%s"<< buf<<endl;     /* print message from server*/
    bzero(buf, BUF_SIZE);

    //send(s, filename, 30, 0); //sends file name to server
    //recv(s, buf, 1, 0);


  	
	        //open file
        FILE *f = fopen(filename, "rb");
        if(NULL == f){
                send(s, "0", 1, 0);
                cout<<"File not found in client\n"<< filename<<endl;
        }else{
                send(s, "1", 1, 0);
                send(s, filename, 30, 0);
                cout<<"Preparing sending file %s\n"<< filename<<endl;
                bzero(buf, BUF_SIZE);
                bytes = 0;
                while((bytes = fread(buf, sizeof(char), BUF_SIZE, f)) > 0){
                        cout<<"file block length is %d\n"<< bytes<<endl;
                        if(send(s, buf, bytes, 0) < 0){
                                fatal("Send error\n");
                        }
                        bzero(buf, BUF_SIZE);
                }

                fclose(f);
                cout<<"Sent file finished.\n"<<endl;
        }


    close(c);   /* close connection*/
    close(s);   /* close socket */

    return 0;
}



