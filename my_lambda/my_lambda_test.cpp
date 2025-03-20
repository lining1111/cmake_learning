//
// Created by lining on 3/20/25.
//
#include <iostream>

using namespace std;

int main() {
    int x = 7;
    float y = 3.0;
    auto f = [x, y](int a, int b) {
        return x * y + a * b;
    };

    cout << f(1, 2) << endl;

    return 0;
}