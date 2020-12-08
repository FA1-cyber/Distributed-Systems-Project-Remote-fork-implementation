#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include "dmtcp.h"
#include "dlfcn.h"
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
using namespace std;
string filename;
string substr1;
string dd ;

struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};
std::string
basename( std::string const& pathname )
{
    return std::string( 
    std::find_if( pathname.rbegin(), pathname.rend(),
    MatchPathSeparator() ).base(),
    pathname.end() );
}

#define dmtcp_get_ckpt_filename() (dmtcp_get_ckpt_filename ? dmtcp_get_ckpt_filename() : NULL);
int myfork(string IPADDRESS)
{
    int r;
   
    string Zipped = "zip -r ZippedFiles.zip *";
    const char *ZIP = Zipped.c_str();
    int original_generation;
    if(dmtcp_is_enabled())
    {
    original_generation=dmtcp_get_generation();
    }
    r = dmtcp_checkpoint();
    std::chrono::seconds dura3(5);
    std::this_thread::sleep_for(dura3);
    filename = dmtcp_get_ckpt_filename();
    dd = basename(filename);
    string rename = "mv " + dd + " testfile.dmtcp";
    const char *RENAME = rename.c_str();
    system(RENAME);
  //  std::chrono::seconds dura(2);
   // std::this_thread::sleep_for(dura);
    //system(ZIP);
    std::chrono::seconds dura2(5);
    std::this_thread::sleep_for(dura2);
    cout<<"A7A"<<rename<<endl;
   
    string StartClient = "g++ client.cpp -o client";
    const char *STARTCLIENT = StartClient.c_str();
    system(STARTCLIENT);
    std::chrono::seconds dura(4);
    std::this_thread::sleep_for(dura);
    string SendFile = "./client "+ IPADDRESS+ " testfile.dmtcp";
    const char *SENDFILES = SendFile.c_str();
    system(SENDFILES);
   
    return r;


}



int main()
{
 
        
            int temp = myfork("10.0.2.15");
            if(temp == DMTCP_AFTER_CHECKPOINT){
                for(int e=0 ; e<10000;e++){
                    cout<<"The current pid is a parent:"<<temp<<endl;
                }
            }
            else if(temp = DMTCP_AFTER_RESTART)
            {
                for(int j=0 ; j<10000;j++){
                    cout<<"The current pid is a child:"<<temp<<endl;
                    
                }
            }
            //sleep(100);
            
            //cout<<dd;
        
     
    
    
    return 0;
}
