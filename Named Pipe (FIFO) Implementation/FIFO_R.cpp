#include<bits/stdc++.h>
#include<sstream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include <sys/stat.h>

using namespace std;

//Utility Function
void invert(string &s){
    int i,j,k,n = s.length();
    for(i=0;i<n;++i){
        if(s[i]>='a' && s[i]<='z'){
            s[i] += ('A'-'a');
        }
        else if(s[i]>'A' && s[i]<='Z'){
            s[i] += ('a'-'A');
        }
    }
    return ;
}

//Conversion Fuction (string <=> char array)
string charToStr(char s[]){
	int i=0;
	string str;
	while(s[i]!='\0'){
		str += s[i]; 
		i++;
	}
	return str;
}


//I/O operation Functions
void setOuput(string strs){
	fstream fs;
	fs.open("output.txt",ios::out);
	fs<<strs;
	fs.close();
}


int main(int argc, char* argv[]){

    string s;

    //opning the fifo file as file desctipter
    int fd = open("fifo_file",O_RDONLY);
    if(fd<0){
        if(errno != EEXIST){
            cout<<"File donesnot exist"<<endl;
            return 1;
        }
        else{
            cout<<"Something bad happend in opening fifo file"<<endl;
            return 2;
        }
    }
    
    char ss[1000];

    //read the fifo file
    if(read(fd,ss,sizeof(ss))<0){
        cout<<"Something bad happend in read the fifo file"<<endl;
        return 3;
    }

    s = charToStr(ss);

    // change the case of all the later
    invert(s);

    cout<<s<<endl;
    // write back output in output.txt file
    setOuput(s);

    return 0;
}