#include <chrono>
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>

#define TIME_SIMULATION 30
#define FLOORS 3
#define TRIP_TIME 3
#define CHECK_TIME_DELAY 4

using namespace std;

int currentFloor;
int moveTo;
int endSimulation;
int busy;
condition_variable cv_request;
mutex m;

void floor_request(int floor) {
    srand(time(NULL));

    do {
        auto request_time = rand() % 3 + 1;
        this_thread::sleep_for(chrono::seconds(request_time));
        
        unique_lock<mutex> lock(m);
        if(floor != currentFloor && !busy)
        {
            cout << "Making request from floor " << floor << endl;
            busy = 1;
            moveTo = floor;
            cv_request.notify_one();
        }
        lock.unlock();

    } while(!endSimulation);
}

void elevator_function(void) {

    do {
        unique_lock<mutex> lock(m);
        while(!busy)
            cv_request.wait(lock);

        cout << "Elevator has received a request, moving..." << endl;
        this_thread::sleep_for(chrono::seconds(TRIP_TIME));
        currentFloor = moveTo;
        busy = 0;
        cout << "Elevator reached floor " << currentFloor << endl;
        lock.unlock();

    } while(!endSimulation);
}

int main(int argc, char const *argv[])
{
    int i, diff;
    auto starting_time = chrono::steady_clock::now();

    cout << "Elevator now at floor 0" << endl; 
    currentFloor = 0;
    endSimulation = 0;
    busy = 0;

    thread elevator(elevator_function);
    elevator.detach();

    for (i = 0; i < FLOORS; i++) {
        thread floor(floor_request, i);
        floor.detach();
    }

    do {
        this_thread::sleep_for(chrono::seconds(CHECK_TIME_DELAY));
        diff = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - starting_time).count();
    } while(diff < TIME_SIMULATION);
    
    endSimulation = 1;

    return 0;
}