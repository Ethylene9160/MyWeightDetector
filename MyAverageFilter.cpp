?
#include "MyAverageFilter.h"
#include<iostream>

void MyAverageFilter::calculateAverage()
{
	average = 0;
	for (int i = 0; i < length; ++i) {
		//std::cout << "the " << i <<"th data in my_queue is: " << my_queue[i] << std::endl;
		average += *(my_queue + i);
		//average += my_queue[i];
	}
	average = average / length;
}

bool MyAverageFilter::checkBalence()
{
	if (isFull) {
		calculateAverage();
		for (int i = 0; i < length; ++i) {
			if (!checkSingleData(*(my_queue + i))) return false;
		}
	}
	return true;
}

bool MyAverageFilter::checkSingleData(mydata data)
{
	mydata absoluteError = abs(data - average);
	if (absoluteError > this->absoluteError) return false;
	if ((demical)absoluteError / (demical)average > relativeError) return false;
	return true;
}

void MyAverageFilter::addElement(mydata data)
{
	*(my_queue + calculator) = data;
	calculator = (++calculator) % length;
}

void MyAverageFilter::reset()
{
	//*my_queue = 0;
	isFull = false;
	isBalence = false;
	average = 0;
	calculator = 0;
	for (int i = 0; i < length; ++i) {
		*(my_queue + i) = 0;
	}
}

MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface) :MyAverageFilter(averageInteface, _MY_AVERAGE_FILTER_)
{

}

MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface, mydata length, mydata absoluteError, demical relativeError)
{
	//todo

	my_queue = new int[length];
	this->averageInteface = averageInteface;

	this->length = length;
	this->absoluteError = absoluteError;
	this->relativeError = relativeError;



	this->averageInteface->show(111);
	//my_queue = new queue<mydata>();
	//*my_queue = 0;
	reset();
}

void MyAverageFilter::update(mydata data)
{
	//calculateAverage();
	//averageInteface->show(average);



	//如果已经平衡了，需要判断平衡是否被打破：
	if (isBalence) {
		//std::cout << "isBalence is ok, check data!\n";
		if (checkSingleData(data)) {
			//std::cout << "new data is ok!\n";
			averageInteface->show(average);
			return;
		}
		else {
			//std::cout << "data is not corect! avg: " << average << "this data: " << data << std::endl;
		}
		//重新配置
		reset();
	}
	else {

		//未平衡
		if (isFull) {
			//my_queue->pop();
			//如果上次未平衡且已经满了，放入元素后
			if (checkBalence()) {
				//std::cout << "isBalence is true!\n";
				isBalence = true;
			}
		}
		else {
			//判断新状态有没有满上
			if (calculator + 1 == this->length) {
				//std::cout << "isFUll is true!\n";
				isFull = true;
			}
		}
		//my_queue->push(data);
	}

	this->addElement(data);
	averageInteface->show(data);
}

MyAverageFilter::MyAverageFilter(AverageInteface* averageInteface, mydata length) :MyAverageFilter(averageInteface, length, 10, 0.05)
{

}
