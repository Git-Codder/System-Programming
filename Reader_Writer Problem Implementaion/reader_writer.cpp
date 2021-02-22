#include<bits/stdc++.h>
#include<thread>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;

//variable declared 
sem_t sem_r; //semaphore to limit maximum 3 threads to read the data or file
mutex mutex_writer,mutex_reader,outPut; //mutex to get mutual exclusion 
int readerCount ; // count of the reader at the time of they reading data

//Function of Read and Write

//Reader will read
void* read(void* y){
    int x = 50;
    while(x--){

        //wait  method calling
        sem_wait(&sem_r);

        //critical section Entry
        mutex_reader.lock(); // lokced reader mutext to enter the critical section 
        readerCount++;
        if(readerCount==1){
            //lock writer if this is first reader 
            mutex_writer.lock();
        }
        mutex_reader.unlock();

        //crtical section Exit
        
        //to get output aligned lock the output mutex
        outPut.lock();
        cout<<"I am reading as reader number : #"<<this_thread::get_id()<<"...."<<endl;
        outPut.unlock();
        // sleep(1); 

        //critical section Entry        
        mutex_reader.lock();
        readerCount--;
        if(readerCount==0){
            //unlock writer to write on data if no reader available
            mutex_writer.unlock();
        }
        mutex_reader.unlock();
        //critical section Exit

        //post method calling 
        sem_post(&sem_r);
    }
    return y;
}

//Writer will write
void* write(void* y){
    int x = 20;
    while(x--){
        //lock the writer mutex to prevent another writer to write on data
        //critical section Enter
        mutex_writer.lock();
        
        cout<<"I am writing as writer number : #"<<this_thread::get_id()<<"...."<<endl;
        sleep(1);

        mutex_writer.unlock();
        //critical section Exit

    }
    return y;
}

int main(int argc, char* argv[]){

    int readerCount = 0;

    //reader semaphore initialisation 
    sem_init(&sem_r,0,3);

    //max limit of rader and writer thread
    int readerNumber = 5;
    int writerNumber = 3;

    //thread vector
    vector<pthread_t> reader(10),writer(5);

    //creating threads
    for(int i=0;i<max(readerNumber,writerNumber);++i){
        if(i<=readerNumber){
            if(pthread_create(&reader[i], NULL, &read, NULL)!=0){
                cout<<"Something error has been occured in creating thred for reader"<<endl;
            }
        }
        if(i<=writerNumber){
           if(pthread_create(&writer[i], NULL, &write, NULL)!=0){
                cout<<"Something error has been occured in creating thred for reader"<<endl;
            }
        }
    }

    for(auto &t : reader){
        pthread_join(t,NULL);
    }

    for(auto &t : writer){
        pthread_join(t,NULL);
    }

    //destroty compelete vector of threads
    reader.clear();
    writer.clear();

    return 0;
}