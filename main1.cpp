#include <iostream>
#include <string>
#include <mutex>
#include <pthread.h>

using namespace std;

enum Activity {
    eat,
    think
};

Activity * activities;
mutex activity_mutex;
mutex * chopsticks;
size_t num_philo;

void eat_philo(int id) {
    if (id == num_philo - 1) {
        chopsticks[0].lock();
        chopsticks[id].lock();
    }
    else {
        chopsticks[id].lock();
        chopsticks[id + 1].lock();
    }
    // eat
    activity_mutex.lock();
    activities[id] = Activity::eat;
    for (int i = 0; i < num_philo; ++i) {
        if (activities[i] == Activity::eat) {
            cout << '*';
            continue;
        }
        cout << ' ';
    }
    cout << "\t\t" <<id << " starts eating" << endl;
    activity_mutex.unlock();
    if (id == num_philo - 1) {
        chopsticks[id].unlock();
        chopsticks[0].unlock();
    }
    else {
        chopsticks[id + 1].unlock();
        chopsticks[id].unlock();
    }
}

void thunk_philo(int id) {
    activity_mutex.lock();
    activities[id] = Activity::think;
    activity_mutex.unlock();
    // thunk
}

void * philosophize(void *threadid) {
    int id = * ((int*) threadid);
    for (int i = 0; i < 3; ++i) {
        thunk_philo(id);
        eat_philo(id);
    }
    return nullptr;
}

int main (int argc, char* argv[]) {
    cout << "Hey there boy-0, give me some phil-0: (# philosophers?)" << endl;
    string user_input;
    getline(cin, user_input);
    try {
        num_philo = stoi(user_input);
    }
    catch(...) {
        cout << "You want to create a number of philosophers that is not an integer?! I must philosphize on this error..." << endl;
        return -1;
    }

    chopsticks = new mutex[num_philo];
    activities = new Activity[num_philo];
    for (size_t i = 0; i < num_philo; ++i) {
        activities[i] = Activity::think;
    }

    pthread_t threads[num_philo];

    for (int i = 0; i < num_philo; ++i) {
        cout << 'T';
    }

    cout << endl;

    for (int i = 0; i < num_philo; ++i) {
        pthread_create(&threads[i], NULL, philosophize, (void *) &i);
    }
    for (int i = 0; i < num_philo; ++i) {
        pthread_join(threads[i], nullptr);
    }

    //create num_philo threads
    //
    cout << num_philo << endl;

    delete [] chopsticks;
    delete [] activities;

    pthread_exit(NULL);
    return 0;
}

