#include<bits/stdc++.h>
#include<syscall.h>
#include<time.h>
#include<thread>
#include <unistd.h>
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
    thread t1(consumer);
    thread t2(producer, product);

    t1.join();
    t2.join();

    return 0;
}