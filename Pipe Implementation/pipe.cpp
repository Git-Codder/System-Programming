#include<bits/stdc++.h>
#include<unistd.h>
#include <sstream>
#include<sys/types.h>
#include<sys/wait.h>

#define buffsize 1000
using namespace std;

//Utility Function 
string removeExtraSpaces(string &str){
	bool first = true,ok = true;
	string s = "";
	int i = 0;
	int n = str.length();
	while(i<n){
		bool ok = false;
		string temp = "";
		while(i<n && str[i]!=' '){
			ok = true;
			temp += str[i];
			if(i>0 && str[i-1]==','){
				temp += " ";
			}
			i++;
		}
		while(i<n && str[i]==' '){
			i++;
		}
		if(i<n && str[i]==','){
			temp += ",";
			i++;
		}
		if(ok){
			s += temp+" ";
		}
	}
	s.pop_back();
	return s;
}

string changeFirstLetter(string &str){
	bool first = true,ok = true;
	string s = "";
	int i = 0;
	int n = str.length();
	while(i<n){
		if(ok){
			if(str[i]>='a' && str[i]<='z'){
				s += (str[i]-'a'+'A');
				ok = false;
			}
			else{
				if(str[i]>='A' && str[i]<='Z'){
					ok = false;
				}
				s += str[i];
			}
		}
		else{
			if(str[i]==' ' || str[i]=='.'){
				s += str[i];
				ok = true;
			}
			else{
				s += str[i];
			}
		}
		i++;
	}

	return s;
}

string stream_as_string(ifstream& stm){
    std::ostringstream str_stm ;
    str_stm << stm.rdbuf() ;
    return str_stm.str() ;
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

void strToChar(char s[],string str){
	int i,n = str.length();
	for(i=0;i<n;++i){
		s[i] = (char)str[i];
	}
	s[i] = '\0';
	return ;
}



//Process Functions
int firstProcess(int fd[][2], int &n){

	//close 1st and 3rd to communicate in 2nd pipe
	int i,j,k;
	int r = 0;
	int w = 1;

	//closing all anothter pipes
	for(i=0;i<n;++i){
		for(j=0;j<2;++j){
			if(!((i==r && j==0)||(i==w && j==1))){
				close(fd[i][j]);
			}
		}
	}

	char str[buffsize];
	if(read(fd[r][0],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in reading pipe msg"<<endl;
		cout<<"1*";
		return 3;
	}

	string strs = charToStr(str);
	strs = removeExtraSpaces(strs);
	strToChar(str,strs);
	// cout<<strs<<endl;
	// cout<<"In First Process...."<<endl;

	if(write(fd[w][1],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in writing msg in pipe"<<endl;
		cout<<"2*";
		return 4;
	}

	close(fd[r][0]);
	close(fd[w][1]);

	return 0;
}

int secondProcess(int fd[][2], int &n){

	//close 1st and 2nd to communicate in 3rd pipe
	int i,j,k;
	int r = 1;
	int w = 2;

	//close all another pipes
	for(i=0;i<n;++i){
		for(j=0;j<2;++j){
			if(!((i==r && j==0)||(i==w && j==1))){
				close(fd[i][j]);
			}
		}
	}

	char str[buffsize];
	if(read(fd[r][0],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in reading pipe msg"<<endl;
		cout<<"3*"<<endl;
		return 6;
	}

	string strs = charToStr(str);
	strs = changeFirstLetter(strs);
	strToChar(str,strs);
	// cout<<strs<<endl;
	// cout<<"In Second Process...."<<endl;

	if(write(fd[w][1],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in writing msg in pipe"<<endl;
		cout<<"4*"<<endl;
		return 7;
	}

	close(fd[r][0]);
	close(fd[w][1]);

	return 0;
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

void setOuput(string strs){
	fstream fs;
	fs.open("output.txt",ios::out);
	fs<<strs;
	fs.close();
}

int main(int argc, char **argv){

	int fd[3][2]; //creating 6 file descriptor for 6 processes
	int i,j,k,n=3;

	for(i=0;i<3;++i){
		if(pipe(fd[i])<0){
			cout<<"An Error has been occured in file descripting"<<endl;
			k = i+1;
			for(i=0;i<k;++i){
				for(j=0;j<2;j++){
					close(fd[i][j]);
				}
			}
			return 1;
		}
	}

	//creating 1st Child Process(2nd Process)
	pid_t p1 = fork();
	if(p1<0){
		cout<<"An Error has beeen occured in creating process using frok"<<endl;
		return 2;
	}

	if(p1==0){

		//In First Child Process
		int n = 3;
		int ret = firstProcess(fd,n);
		return ret;
	}
	
	//Creating 2nd child Process (3rd Process)
	pid_t p2 = fork();
	if(p2<0){
		cout<<"An Error has beeen occured in creating process using frok"<<endl;
		return 5;
	}
	if(p2==0){
		
		//In Second Child Process
		int n = 3;
		int ret = secondProcess(fd,n);
		return ret;
	}


	//Perfroming action in main Process(1st Process)
	int r = 2;
	int w = 0;
	for(i=0;i<n;++i){
		for(j=0;j<2;++j){
			if(!((i==r && j==0 || i==w && j==1))){
				close(fd[i][j]);
			}
		}
	}


	// getting Input From File
	string strs = getInput();


	char str[buffsize];
	strToChar(str,strs);

	if(write(fd[w][1],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in writing msg in pipe"<<endl;
		cout<<"5*"<<endl;
		return 8;
	}

	if(read(fd[r][0],str,sizeof(str)) < 0){
		cout<<"An Error has been occured in reading pipe msg"<<endl;
		cout<<"6*"<<endl;
		return 9;
	}

	close(fd[w][1]);
	close(fd[r][0]);

	strs = charToStr(str);

	//Save Output in File 
	setOuput(strs);

	//Wating for compelete to child process
	waitpid(p1,NULL,0);
	waitpid(p2,NULL,0);

	return 0;
}