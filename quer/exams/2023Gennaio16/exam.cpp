#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex m_cv, m_v;
condition_variable cv;
vector<int> v;
bool end_of_prog;
int command;

void print_vector() {
    auto i = v.begin();

    cout << "array content: ";
    for(; i != v.end(); i++)
        cout << (*i) << ", ";
    cout << endl;
}

void print_last() {
    auto i = v.end();
    cout << "last element of the vector is: " << (*i) << endl;
}

void delete_all() {
    unique_lock<mutex> lock_v(m_v);
    v.clear();
}

void writer() {
    srand(static_cast<unsigned int>::get_id());

    while (!end_of_prog) {
        this_thread::sleep_for(chrono::seconds(5));
        
        unique_lock<mutex> lock_v(m_v);
        v.emplace_back(rand() % 10 + 1);
        lock_v.unlock();
    }
}

void worker() {
    while(!end_of_prog) {
        unique_lock<mutex> my_lock(m_cv);
        
        while (command == -1)
            cv.wait(my_lock);

        switch (command)
        {
            case 1:
                print_vector();
                break;

            case 2:
                print_last();
                break;
            
            case 3:
                delete_all();
                break;

            default:
                break;
        }

        command = -1;
    }
}

void ui() {
    while (!end_of_prog) {
        cin >> command;
        switch(command) {
            case 0:
            default: 
                end_of_prog = 1;
                break;
        }
        cv.notify_one();
    }
}

int main(int argc, char const *argv[])
{
    command = -1;
    end_of_prog = 0;

    thread writer_t(writer);
    thread worker_t(worker);
    thread ui_t(ui);

    writer_t.join();
    worker_t.join();
    ui_t.join()

    return 0;
}
