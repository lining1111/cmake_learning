//
// Created by lining on 3/20/25.
//
#include <memory>
#include <iostream>
#include "vector"
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;


class Packet {

public:
    Packet(long id) : m_id(id) {

    }

public:
    long m_id;
    char data[100];
};

class Compare {
public:
    bool operator()(const Packet &p1, const Packet &p2) {
        return p1.m_id < p2.m_id;
    }

    bool operator()(const Packet *p1, const Packet *p2) {
        return p1->m_id < p2->m_id;
    }

    template<template<typename> typename SmartPtr>
    bool operator()(const SmartPtr<Packet> &p1, const SmartPtr<Packet> &p2) {
        return p1->m_id < p2->m_id;
    }
};

class AutoTimer {
private:
    high_resolution_clock::time_point startTime;
    string description;
public:
    AutoTimer(const string &description) : description(description) {
        startTime = high_resolution_clock::now();
    }

    ~AutoTimer() {
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime);
        cout << description << " cost " << duration.count() << " us" << endl;
    }
};

void sortValueVector(vector<int> ids) {
    vector<Packet> vecPackets;
    for (auto id: ids) {
        vecPackets.push_back(Packet(id));
    }
    {
        AutoTimer timer("sortValueVector");
        sort(vecPackets.begin(), vecPackets.end(), Compare());
    }
}

void sortPointerVector(vector<int> ids) {
    vector<Packet *> vecPackets;
    for (auto id: ids) {
        vecPackets.push_back(new Packet(id));
    }
    {
        AutoTimer timer("sortPointerVector");
        sort(vecPackets.begin(), vecPackets.end(), Compare());
    }
}

template<typename SmartPrt>
void sortSmartPointerVector(vector<int> ids) {
    vector<SmartPrt> vecPackets;
    for (auto id: ids) {
        vecPackets.push_back(SmartPrt(new Packet(id)));
    }
    {
        AutoTimer timer("sortSmartPointerVector");
        sort(vecPackets.begin(), vecPackets.end(), Compare());
    }
};

int main() {
    int n = 10000;
    vector<int> randomId(n, 0);
    for (int i = 0; i < n; i++) {
        randomId[i] = rand() % 100000;
    }
    sortValueVector(randomId);
    sortPointerVector(randomId);
    sortSmartPointerVector<unique_ptr<Packet>>(randomId);
    return 0;
}
