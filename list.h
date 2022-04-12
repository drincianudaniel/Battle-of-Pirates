#pragma once
template <typename T>
class List
{
public:
	List();
	~List();
	List(const List& rhs);
	List& operator=(const List& rhs);
	bool isEmpty();
	LinkedNode* getFirst();
	LinkedNode* getLast();
	void insertFirst(T data);
	void insertLast(T data);
	void insertAfter(T tKey, T tData);
	void removeFirst();
	void removeLast();
	void remove(T removalCandidate);
	void destroy();
private:
	LinkedNode* mFirst;
	LinkedNode* mLast;
};