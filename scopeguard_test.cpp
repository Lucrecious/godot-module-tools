#include <iostream>

#include "scopeguard.hpp"

using namespace std;

int main(int cargs, char** args) {
    ScopeGuard guard([](){ cout << "main - finished" << endl; });

    int scopeguard_count = 0;
    int loop_count = 0;
    while (loop_count < 10) {
        loop_count++;
        ScopeGuard([&](){ scopeguard_count++; });
    }

    if (scopeguard_count != loop_count) {
        cout << "Scope guard doesn't work in loops" << endl;
    }
    

    cout << "main - right before deserting" << endl;
    return 0;
}