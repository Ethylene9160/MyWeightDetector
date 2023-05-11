#include <iostream>
#include "MyAverageFilter.h"


class Test : public AverageInteface {
private:
    MyAverageFilter* filter;

public:
    Test() {
        filter = new MyAverageFilter(this);
    }
    // 通过 AverageInteface 继承
    virtual void show(int data) override
    {
        std::cout << "weight of which is: " << data << std::endl;
    }

    void update(mydata d) {
        filter->update(d);
    }
};

int main()
{
    Test* t = new Test();
    //MyAverageFilter* filter = new MyAverageFilter((AverageInteface*)t);

    queue<int> myqs;
    cout << "******\n";
    myqs.push(1);
    myqs.push(2);
    myqs.push(3);
    while (!myqs.empty()) {
        std::cout << myqs.front() << std::endl;
        myqs.pop();
    }

    cout << "** filter test! **\n";
    t->update(100);
    t->update(200);
    t->update(300);
    t->update(203);
    t->update(199);
    t->update(202);
    t->update(198);
    t->update(199);
    t->update(205);
    t->update(205);

    t->update(5);
    t->update(4);
    t->update(6);
    t->update(6);
    t->update(5);
    t->update(4);


    t->update(51);
    t->update(51);
    t->update(52);
    t->update(52);
    t->update(50);
    t->update(50);


    delete t;
    //delete filter;
    return 0;
}
