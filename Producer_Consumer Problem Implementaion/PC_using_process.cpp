#include<bits/stdc++.h>
#include<syscall.h>
#include<time.h>
#include <unistd.h>
#include <errno.h> 
#include<sys/wait.h>

using namespace std;

mutex m;
condition_variable cv;
deque<int> productBuffer;
const int maxBufferSize = 10;


//Producer producing 
void producer(int product){
    while(product){
        unique_lock<mutex> locker(m);
        cv.wait(locker,[](){return productBuffer.size()<maxBufferSize;});

        cout<<"Producer is producing Product #: "<<product<<" ......."<<endl;
        sleep(1);
        productBuffer.push_back(product);
        product--;

        locker.unlock();
        cv.notify_one();
    }
    return ;
}


//Consumer consuming
void consumer(){
    while(1){
        unique_lock<mutex> locker(m);
        cv.wait(locker,[](){return productBuffer.size()>0;});

        cout<<"Consumer is consuming Product #: "<<productBuffer.front()<<" ...."<<endl;
        sleep(1);
        productBuffer.pop_front();

        locker.unlock();
        cv.notify_one();
    }
    return ;
}


int main(int argc, char* argv[]){
    
    int product = 50;

    pid_t p1 = fork();

    if(p1<0){
        cout<<"soething error has been occured in creating process 1!"<<endl;
        return 1;
    }
    else if(p1==0){
        consumer();
        return 0;
    }
    
    pid_t p2 = fork();

    if(p2<0){
        cout<<"soething error has been occured in creating process 2!"<<endl;
        return 2;
    }
    else if(p2==0){
        producer(product);
        return 0;
    }

    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);

    return 0;
}