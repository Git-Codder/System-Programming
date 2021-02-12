#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sstream>
#include<string.h>
#include<syscall.h>

using namespace std;

//convert string object in char string
char** converter(string s, int &sz){
    int i=0;

    //create string that has all char of string object
    char *st = (char *)malloc(sizeof(char)*sizeof(s));
    for(i=0;i<s.length();++i){
        st[i] = s[i];
    }
    
    //create 2-d char array(array of string) to keep all variable
    char** str = (char **)malloc(sizeof(char*)*sz);
    char *token = strtok(st," ");
    i=0;
    while (token != NULL){
        
        str[i] = (char *)malloc(sizeof(char)*sizeof(token));
        str[i] = token;
    
        token = strtok(NULL," ");
        i++;
    }
    sz = i;

    //free extra allocated space
    for(int i=sz;i<10;++i){
        free(str[i]);
    }

    return str;
}

//get full binary path for command
char* getPath(char* command){

    //creating constant directory string
    char *dir = (char *)malloc(sizeof(char )*sizeof("/bin/"));
    dir = (char *)("/bin/");

    //careating string conataining commad with path
    int total_size = sizeof("/bin/") + sizeof(command);
    char *path = (char *)malloc(sizeof(char)*total_size);
    
    int len = sizeof(path)/sizeof(char);
    for(int i=0;i<len;++i){
        if(i<5){
            path[i] = dir[i];
            continue;
        }
        path[i] = command[i-5];
    }

    return path;
}

int main(int argc, char const *argv[]){

    while(1){
    
        //create pipe to check status
        int fd[2][2];
        for(int i=0;i<2;++i){
            if(pipe(fd[i])<0){
                cout<<"Error has been occured in creating pipe"<<endl;
                return 1;
            }
        }

        //creating child process to run the command
        pid_t p1 = fork();

        if(p1<0){
            cout<<"Error has been occured in creating process"<<endl;
            return 2;
        }

        if(p1==0){
            cout<<">>> ";
            // In child Process
            
            int sz = 10;

            //getting input from terminal as whole string 
            string s;
            getline(cin,s);
            

            //command line arguments
            char** args = converter(s,sz);

            if(sz==1){

                char* ex = (char *)("exit");
                if(strcmp(args[0],ex)==0){

                    close(fd[0][1]);
                    close(fd[1][0]);
                    int x ;
                    if(read(fd[0][0],&x,sizeof(x))<0){
                        cout<<"Error has been occured in read pipe"<<endl;
                        return 3;
                    }
                    x = -1;
                    if(write(fd[1][1],&x,sizeof(x))<0){
                        cout<<"Error has been occured in write pipe"<<endl;
                        return 4;
                    }
                    close(fd[0][0]);
                    close(fd[1][1]);
                    return 0;
                }
            }
            char *bin_path = getPath(args[0]);
            args[0] = bin_path;

            execv(bin_path,args);
            
        }

        //check for exit command
        close(fd[0][0]);
        close(fd[1][1]);

        int x = 0;
        if(write(fd[0][1],&x,sizeof(x))<0){
            cout<<"Error has been occured in write on pipe"<<endl;
            return 5;
        }

        if(read(fd[1][0],&x,sizeof(x))<0){
            cout<<"Error has been occured in read pipe"<<endl;
            return 6;
        }

        close(fd[0][1]);
        close(fd[1][0]);

        if(x==-1){
            cout<<"Good Bye"<<endl;
            return 0;
        }
        
        //wait for child
        wait(NULL);

    }
    return 0;
}
