//
// Created by lining on 3/20/25.
//
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Person {
private:
    string _name;
//    shared_ptr<Person> _partner;//shared_ptr在相互引用下无法自动销毁
    weak_ptr<Person> _partner;//weak_ptr在相互引用下无法自动销毁
public:
    Person(const string &name) : _name(name) {
        cout << _name << " constructed" << endl;
    }

    ~Person() {
        cout << _name << " destructed" << endl;
    }

    void setPartner(const shared_ptr<Person> &other) {
        _partner = other;
    }
};


void testhuxiang(){
        vector<shared_ptr<Person>> people;
        people.push_back(shared_ptr<Person>(new Person("张三")));
        people.push_back(shared_ptr<Person>(new Person("李四")));
        people.push_back(shared_ptr<Person>(new Person("王五")));
        people[0]->setPartner(people[1]);
        people[1]->setPartner(people[2]);
        people[2]->setPartner(people[0]);
}

class Rect {
};

void testGetOwner() {
    weak_ptr<Rect> w_p1;
    {
        shared_ptr<Rect> s_p1(new Rect());
        shared_ptr<Rect> s_p2 = s_p1;
        w_p1 = s_p2;
        shared_ptr<Rect> s_p3 = w_p1.lock();
        cout << "作用域内s_p3=" << s_p3 << endl;
    }
    shared_ptr<Rect> s_p3 = w_p1.lock();
    cout << "作用域外s_p3=" << s_p3 << endl;
}


int main() {
//    testhuxiang();
    testGetOwner();

    return 0;
}