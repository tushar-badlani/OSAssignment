#include<iostream>
#include<vector>
#include<ctime>
#include<semaphore.h>
#include<pthread.h>
#include<cstdlib>
#include<unistd.h>

using namespace std;

int s, rc=0;
int shared=0;
int nr, nw;
pthread_mutex_t count, print, rw;
sem_t wr;


void print_read(int id){
    usleep(10000+rand()%10000);
    pthread_mutex_lock(&print);
    cout << "Reader " << id << " is reading shared variable: " << shared << endl;
    pthread_mutex_unlock(&print);
    usleep(10000+rand()%10000);
}

void print_writer(int id){
    usleep(10000+rand()%10000);
    shared++;
    pthread_mutex_lock(&print);
    cout << "Writer " << id << " is writing into shared variable " << shared << endl;
    pthread_mutex_unlock(&print);
    usleep(10000+rand()%10000);
}


void* reader(void* arg){
    int id = long(arg);

    if(s==0){
        print_read(id);
    }
    else if(s ==1){
        pthread_mutex_lock(&rw);
        print_read(id);
        pthread_mutex_unlock(&rw);
    }
    else if(s ==2){
        sem_wait(&wr);
        print_read(id);
        sem_post(&wr);
    }
    else if(s ==3){
        pthread_mutex_lock(&count);
        rc++;
        if(rc==1){
            sem_wait(&wr);
        }
        pthread_mutex_unlock(&count);

        print_read(id);
        
        pthread_mutex_lock(&count);
        rc--;
        if(rc==0){
            sem_post(&wr);
        }
        pthread_mutex_unlock(&count);
    }
    return nullptr;
}


void* writer(void* arg){
    int id = long(arg);

    if(s ==0){
        print_writer(id);
    }
    else if(s ==1){
        pthread_mutex_lock(&rw);
        print_writer(id);
        pthread_mutex_unlock(&rw);
    }
    else if(s == 2){
        sem_wait(&wr);
        print_writer(id);
        sem_post(&wr);
    }
    else if(s ==3){
        sem_wait(&wr);
        print_writer(id);
        sem_post(&wr);
    }
    return nullptr;
}


int main(){
    cout << "Enter operation 0 no lock 1 mutex 2 semaphore 3 correct: ";
    cin >> s;

    cout << "Enter num of readers: ";
    cin >> nr;

    cout << "Enter number of writers: ";
    cin >> nw;

    vector<char> v;

    for(int i=0; i<nw; i++){
        v.push_back('w');
    }

    for(int i=0; i<nr; i++){
        v.push_back('r');
    }

    for(int i=0; i<50; i++){
        int a = rand()%v.size();
        int b = rand()%v.size();

        swap(v[a], v[b]);
    }

    pthread_mutex_init(&count , nullptr);
    pthread_mutex_init(&print, nullptr);
    pthread_mutex_init(&rw, nullptr);
    sem_init(&wr, 0,1);

    vector<pthread_t> threads(nr+nw);

    int ri=1, wi=1;
    for(int i=0; i<v.size(); i++){
        if(v[i]=='r'){
            pthread_create(&threads[i], nullptr, reader, (void*)(long)ri++);
        }
        else{
            pthread_create(&threads[i], nullptr, writer, (void*)(long)wi++);
        }
    }

    for(auto t: threads){
        pthread_join(t, nullptr);
    }

    pthread_mutex_destroy(&count);
    pthread_mutex_destroy(&print);
    pthread_mutex_destroy(&rw);
    sem_destroy(&wr);

    return 0;

}