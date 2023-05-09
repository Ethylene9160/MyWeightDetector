#ifndef _MY_AVERAGE_FILTER_
#define _MY_AVERAGE_FILTER_ 3
#include<queue>
typedef double demical;
typedef int mydata;
using namespace std;
class AverageInteface {
public:
	virtual void show(mydata data) = 0;
};

class MyAverageFilter {
private:
	mydata length;
	mydata average;
	mydata absoluteError;
	demical relativeError;
	AverageInteface* averageInteface;
	queue<mydata> *my_queue;
	bool isFull;
	bool isBalence;

	void calculateAverage();

	bool is();
public:
	MyAverageFilter(AverageInteface* averageInteface);

	MyAverageFilter(AverageInteface* averageInteface, mydata length);

	MyAverageFilter(AverageInteface* averageInteface, mydata length, mydata absoluteError, demical relativeError);

	void update(mydata data);

	

	
};

#endif