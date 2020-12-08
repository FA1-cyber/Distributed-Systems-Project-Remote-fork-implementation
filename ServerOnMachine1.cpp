#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>                     /* add by Kun*/
#include <stdio.h>                      /* add by Kun*/
#include <stdlib.h>                     /* add by Kun*/
#include <unistd.h>
#include <iostream>
using namespace std;
#define SERVER_PORT 12345               /* arbitrary, but client and server must agree */
#define BUF_SIZE 10240                   /* block transfer size */
#define QUEUE_SIZE 10
 void fatal(string ss)
{
    cout<< ss<<endl;
    exit(1);
}

int main(int argc, char *argv[])
{
      

    int s, b, l, sa, bytes, on = 1;
    char buf[BUF_SIZE];                   /* buffer for outgoing file */
    char filename[300];
    struct sockaddr_in channel;           /* hold's IP address */

    memset(&channel, 0, sizeof(channel)); /* zero channel */
    channel.sin_family = AF_INET;
    channel.sin_addr.s_addr = htonl(INADDR_ANY);
    channel.sin_port = htons(SERVER_PORT);

    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
          fatal("socket creation failed\n");
    }else{
          cout<<"socket creation successful\n"<<endl;
    }
  
  
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    //bind
    if ((b = bind(s, (struct sockaddr *) &channel, sizeof(channel))) < 0){
          fatal("bind failed\n");
    }else{
          cout<<"bind successfully\n"<<endl;
    }

    //listen
    if ((l = listen(s, QUEUE_SIZE)) < 0){
          fatal("listen failed\n");
    }else{
          cout<<"listen successfully\n"<<endl;
    }
   int fbl = 0;
    //accept
    while (1) {
        if ((sa = accept(s, 0, 0)) < 0){
                fatal("accept failed\n");
        }else{
                cout<<"accept successfully\n"<<endl;
        }

        send(sa, "Welcome to TCP server\n", 22, 0);   /*send welcome message to client, added by Kun*/


    //send(s, filename, 30, 0);
    recv(sa, buf, 1, 0); 
    recv(sa, filename, 30, 0); 

   
    /*if(strcmp(buf, "1") == 0){
          cout<<"File %s found on Client\n"<< filename<<endl;
    }else{
          cout<<"File %s not found on Client\n"<< filename<<endl;
          exit(0);
    }
    
  /* Connection is now established. receive file name including 0 byte at end. */
    
    FILE *f = fopen(filename, "wb+");
    if(f == NULL){
          fatal("File not found at client\n");
    }else{
          cout<<"Preparing receiving file \n"<< filename<<endl;
          bzero(buf, BUF_SIZE);    /* make buf empty before use it*/
          bytes = 0;

          while(bytes = recv(sa, buf, BUF_SIZE, 0)){
                  if(bytes < 0){
                          fatal("Receive Error!\n");
                  }

                  fbl = fwrite(buf, sizeof(char), bytes, f);
                  if(fbl < bytes){
                          fatal("Write File Error!\n");
                  }

                  bzero(buf, BUF_SIZE);
          }

          fclose(f);
          cout<<"File %s is received from client \n"<< filename<<endl;
    }

    //string str1 = "dmtcp_restart *.dmtcp";
    //const char * runcommand = str1.c_str();
    //system(runcommand);

    //string str2 = "unzip ZippedFiles.zip";
    //const char * unzipfiles = str2.c_str();
   // system(unzipfiles);
    //sleep(10);
    string str3 = "dmtcp_restart testfile.dmtcp";
    const char * restartdmtcp = str3.c_str();
    system(restartdmtcp);
    close(sa);                               /* close connection */
    }
    
    close(s);


}
