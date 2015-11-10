//
//  main.cpp
//  Philosophers
//
//  Created by Morgan Redding on 11/9/15.
//  Copyright © 2015 Morgan Redding. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <unistd.h>

enum Activity {
    eating = '*',
    thinking = ' '
};

std::mutex* chopsticks;
std::mutex activity_lock;

Activity* activities;

size_t n = 10;

void print_activities() {
    for (size_t i = 0; i < n; i++) {
        std::cout << char(activities[i]) << "  ";
    }
}

void* philospher(void* args) {
    
    size_t id = *(size_t*)args;
    size_t first_chopstick = (id == n - 1 ? 0 : id);
    size_t second_chopstick = (id == n - 1 ? id : id + 1);
    
    
    for (size_t i = 0; i < 3; i++) {
        chopsticks[first_chopstick].lock();
        chopsticks[second_chopstick].lock();
        
        activity_lock.lock();
        activities[id] = eating;
        print_activities();
        std::cout << id << "  starts eating" << std::endl;
        activity_lock.unlock();
        
        // eat
        usleep(rand() % 1000000);
        
        chopsticks[second_chopstick].unlock();
        chopsticks[first_chopstick].unlock();
        
        activity_lock.lock();
        activities[id] = thinking;
        print_activities();
        std::cout << id << "   stops eating" << std::endl;
        activity_lock.unlock();
        
        // think
        usleep(rand() % 1000000);
    }
    
    return nullptr;
}

int main(int argc, const char * argv[]) {
    
    pthread_t* threads = new pthread_t[n];
    size_t* ids = new size_t[n];
    chopsticks = new std::mutex[n];
    activities = new Activity[n];
    
    for (size_t i = 0; i < n; i++) {
        activities[i] = thinking;
        if (std::to_string(i).size() == 1) {
            std::cout << i << "  ";
        }
        else {
            std::cout << i << ' ';
        }
    }
    std::cout << std::endl;
    for (size_t i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, philospher, (void*)&ids[i]);
    }
    for (size_t i = 0; i < n; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    delete [] threads;
    delete [] ids;
    delete [] chopsticks;
    delete [] activities;
    
    return 0;
}
