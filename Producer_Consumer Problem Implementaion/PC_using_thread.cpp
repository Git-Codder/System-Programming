#include<bits/stdc++.h>
#include<syscall.h>
#include<time.h>
#include<thread>
#include <unistd.h>
#include<sys/wait.h>

using namespace std;

//declare variable
mutex m, lockPthread; // mutex to get mutual exclusion
condition_variable cv; //condition variable to get the functionality of notify_one and notify_all
deque<int> productBuffer; // Buffer as a queue
const int maxBufferSize = 10; // Buffer size of products
int producerThread ; // total number of thread that all will notify by incrementing this variable to prevent consumer for more wait
int produced ; // variable to prevent the consumer to wait more if there will be no production in future

//Producer producing 
void producer(int product){
    while(product){
        //using unique_lock method to acquire the mutex
        //crtical section Entry
        unique_lock<mutex> locker(m);
        cv.wait(locker,[](){return productBuffer.size()<maxBufferSize;}); //checking condition if production is possible or not

        cout<<"Producer is producing Product #: "<<product<<" ....."<<endl;
        // sleep(1);

        productBuffer.push_back(product);
        product--;

        locker.unlock();
        cv.notify_one();
        //crtical section Exit
    }
    
    // critical section Entry (It handle the case of more than one thread of producer)
    lockPthread.lock();
    produced++;
    lockPthread.unlock();
    //critical sectino Exit

    return ;
}


//Consumer consuming
void consumer(){
    bool flag = false;
    while(1){
        //using unique_lock method to acquire the mutex
        //crtical section Entry
        unique_lock<mutex> locker(m);
        
        //check condition when no product reamaing in buffer and all producer has been produced , in future no production 
        if(producerThread==produced && productBuffer.size()==0){
            flag = true;
            goto label ;
        }
        cv.wait(locker,[](){return productBuffer.size()>0;}); //checking condition if Consumption is possible or not

        cout<<"Consumer is consuming Product #: "<<productBuffer.front()<<" ....."<<endl;
        // sleep(1);
        productBuffer.pop_front();

        label :
        locker.unlock();
        if(flag){
            break; // break the loop if noproduction in future
        }

        cv.notify_one();
        //critical section Exit
    }
    return ;
}


int main(int argc, char* argv[]){
    
    //limit the product number
    int product = 50;
    producerThread = 1;
    produced = 0;

    // create the thread
    thread t1(consumer);    
    thread t2(producer, product);

    t1.join();
    t2.join();

    return 0;
}