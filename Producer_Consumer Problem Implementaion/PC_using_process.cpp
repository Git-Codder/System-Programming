#include<bits/stdc++.h>
#include<syscall.h>
#include<time.h>
#include <unistd.h>
#include <errno.h> 
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/types.h>

using namespace std;

const int maxBufferSize = 10; // Buffer size of products

struct Packet
{
    //declare variable
    mutex m; // mutex to get mutual exclusion
    condition_variable cv; //condition variable to get the functionality of notify_one and notify_all
    int productBuffer[maxBufferSize]; // Buffer as a queue
    int productIndex ; // index to track the product in product Buffer
} firstProductPackate;

//Producer producing 
void producer(int product, Packet& productPackate){
    while(product){
        //using unique_lock method to acquire the mutex
        //crtical section Entry
        unique_lock<mutex> locker(productPackate.m);
        int idx = productPackate.productIndex+1;
        productPackate.cv.wait(locker,[&](){return idx<maxBufferSize;}); //checking condition if production is possible or not

        cout<<"Producer is producing Product #: "<<product<<" ....."<<endl;
        // sleep(1);

        productPackate.productIndex++;
        productPackate.productBuffer[idx] = product;
        product--;

        locker.unlock();
        productPackate.cv.notify_one();
        //crtical section Exit
    }
    return ;
}


//Consumer consuming
void consumer(Packet& productPackate){
    while(1){
        //using unique_lock method to acquire the mutex
        //crtical section Entry
        unique_lock<mutex> locker(productPackate.m);
        int idx = productPackate.productIndex;
        productPackate.cv.wait(locker,[&](){return idx>0;}); //checking condition if Consumption is possible or not

        cout<<"Consumer is consuming Product #: "<<productPackate.productBuffer[productPackate.productIndex]<<" ....."<<endl;
        // sleep(1);
        productPackate.productIndex--;

        locker.unlock();
        productPackate.cv.notify_one();
        //critical section Exit
    }
    return ;
}

//creation of shared memory
int createSharedMemory(){
    int key = 9876;
    int shmid  = shmget(key, sizeof(firstProductPackate), IPC_CREAT | 0666);

    if(shmid < 0){
        cout<<"Error has been occured in creating shared memory"<<endl;
        return -1;
    }
    return shmid;
}

//attach sharedMemory segment with current process
Packet* attachMemory(int &shmid){
    Packet* shm = (Packet* )shmat(shmid, NULL, 0);
    if(shm == (Packet* )-1){
        cout<<"Error has been occured in attaching shared memory"<<endl;
        shmid = -1;
    }
    return shm;
}

//dettach sharedMemory from current process
int dettachMemory(Packet* shm){
    int x = shmdt(shm);
    if(x<0){
        cout<<"Error has been occured in dettaching memory"<<endl;
    }
    return x;
}

//destroy shared memory if exists
void destrySharedMemory(int shmid){
    int key = shmid;
    if(-1 == (shmctl(key, IPC_RMID, NULL))){   
        cout<<"Error has been occured in destroying shared memory"<<endl;
    } 
    return ;
}


int main(int argc, char* argv[]){
    
    //limit the product number 
    int product = 5;

    //create first process
    pid_t p1 = fork();
    // int p1 = 1;

    if(p1<0){
        cout<<"soething error has been occured in creating process 1!"<<endl;
        return 1;
    }
    else if(p1==0){
        int shmid  = createSharedMemory();
        if(shmid<0){
            return 0;
        }

        Packet* shm = attachMemory(shmid);
        if(shmid<0){
            return 0;
        }

        consumer(*shm);

        int x = dettachMemory(shm);
        return 0;
    }
    
    //create second process
    pid_t p2 = fork();
    // int p2 = 1;

    if(p2<0){
        cout<<"soething error has been occured in creating process 2!"<<endl;
        return 2;
    }
    else if(p2==0){
        int shmid  = createSharedMemory();
        if(shmid<0){
            return 0;
        }

        Packet* shm = attachMemory(shmid);
        if(shmid<0){
            return 0;
        }

        producer(product, *shm);

        int x = dettachMemory(shm);
        return 0;
    }

    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);

    // int shmid  = createSharedMemory();
    // if(shmid<0){
    //     return 0;
    // }
    // destrySharedMemory(shmid);

    return 0;
}