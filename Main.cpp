#include <iostream>
#include "MyAverageFilter.h"


class Test : AverageInteface {
    // Í¨¹ý AverageInteface ¼Ì³Ð
    virtual void show(int data) override
    {
        std::cout << "weight of which is: " <<data<< std::endl;
    }
};




int main()
{
    Test* t = new Test();
    MyAverageFilter* filter = new MyAverageFilter((AverageInteface*)t);

    queue<int> myqs;
    cout << "******\n";
    myqs.push(1);
    myqs.push(2);
    myqs.push(3);
    while (!myqs.empty()) {
        std::cout << myqs.front() << std::endl;
        myqs.pop();
    }

    delete t;
    delete filter;
    return 0;
}
