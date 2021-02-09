#include<bits/stdc++.h>
#include<sstream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include <sys/stat.h>

using namespace std;

//Conversion Fuction (string <=> char array)
void strToChar(char s[],string str){
	int i,n = str.length();
	for(i=0;i<n;++i){
		s[i] = (char)str[i];
	}
	s[i] = '\0';
	return ;
}

//I/O operation Functions
string getInput(){
	fstream fs;
	fs.open("input.txt"	,ios::in);

	string str = "";
	// str = stream_as_string(fs);
	while (fs.good())
        getline(fs, str); 
	fs.close();

	return str;
}

int main(int argc, char* argv[]){

    string s = getInput();

    //creating fifo file 
    if(mkfifo("fifo_file",0666)){
        if(errno != EEXIST){
            cout<<"Could not create FIFO File "<<endl;
            return 1;
        }
        else{
            cout<<"File already exixts in the sytem"<<endl;
            cout<<"For Execute Programm Enter : 1 "<<endl;
            cout<<"For Exit Programm          : 0 "<<endl;

            int x ;
            cin>>x;
            if(!x){
                cout<<"Programm terminated !"<<endl;
                return 0;
            }
        }

    }

    //opening the fifo file as file descriptor
    int fd = open("fifo_file",O_WRONLY);
    if(fd<0){
        cout<<"Error has been occured in opening fifo file"<<endl;
        return 2;
    }
    else{
        //writing in fifo file
        int sz = s.length();
        char ss[sz];
        strToChar(ss,s);

        if(write(fd,ss,sizeof(ss))<0){
            cout<<"something bad happened in writing fifo file"<<endl;
            return 3;
        }
    }

    return 0;
}