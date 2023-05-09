
#include "MyAverageFilter.h"


MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface):MyAverageFilter(averageInteface, _MY_AVERAGE_FILTER_)
{

}

MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface, mydata length, mydata absoluteError, demical relativeError)
{
	//todo

	this->averageInteface = averageInteface;

	this->length = length;
	this->absoluteError = absoluteError;
	this->relativeError = relativeError;

	this->averageInteface->show(111);
	my_queue = new queue<mydata>();
}

void MyAverageFilter::update(mydata data)
{
	

	//如果已经平衡了，需要判断平衡是否被打破：
	if (isBalence) {

	}
	else {
		//未平衡
		if (isFull) {
			my_queue->pop();
		}
		my_queue->push(data);
	}
}

MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface, mydata length):MyAverageFilter(averageInteface, length, 10, 0.05)
{
	
}
