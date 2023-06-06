#define PARKING_SLOTS 3
#define TIME_SIMULATION 10

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

int count = 0;
mutex m;

void car_function(void) {
    unique_lock<mutex> lock(m);
    if (count == PARKING_SLOTS)
    {
        cout << "Parcheggio pieno" << endl;
        return;
    }
    else 
    {
        count++;
        cout << "Arrivata nuova macchina: " << PARKING_SLOTS - count << " posti liberi su "  << PARKING_SLOTS << endl;
    }
    lock.unlock();

    auto parking_waiting_time = rand() % 4 + 4;
    this_thread::sleep_for(chrono::seconds(parking_waiting_time));

    lock.lock();
    count--;
    cout << "Esce macchina: " << PARKING_SLOTS - count << " posti liberi su "  << PARKING_SLOTS << endl;
    lock.unlock();

    return;
}

int main(int argc, char const *argv[])
{
    int diff;
    bool active = false;
    decltype(chrono::steady_clock::now()) starting_time;

    srand(time(NULL));

    do {
        auto queue_waiting_time = rand() % 3 + 1;
        this_thread::sleep_for(chrono::seconds(queue_waiting_time));

        if (!active)  {
            starting_time = chrono::steady_clock::now();
            active = true;
        }

        thread car(car_function);
        car.detach();

        diff = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - starting_time).count();
        cout << "Sono passati " << diff << " secondi su "  << TIME_SIMULATION << endl;
    }
    while(diff < TIME_SIMULATION);

    while(count) this_thread::sleep_for(chrono::seconds(1));

    return 0;
}
