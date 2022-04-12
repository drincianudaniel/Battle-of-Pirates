#include "list"
#include <list>
#include <iostream>
using namespace std;
int main() {

	list<int> myIntList;
	myIntList.push_front(4);
	myIntList.push_front(3);
	myIntList.push_front(2);
	myIntList.push_front(1);
	return 0;
}