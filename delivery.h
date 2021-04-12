#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream> 

using namespace std;

// Узел списка записей о выдаче симок
struct Record
{
	string passport;
	string number;
	string start_date;
	string finish_date;
	bool is_returned; // fals - выдали, true - вернули 
	Record* next;

	Record() : next(nullptr), is_returned(false)
	{
	}

	Record(string pass, string num, string start, string finish) :
		passport(pass), number(num), start_date(start), finish_date(finish),
		is_returned(false), next(nullptr)
	{
	}

	void load(istream& in);
	void input();
	friend ostream& operator<<(ostream& out, Record n);
};
//------------------------------------------------------

// Прототипы функций для СОРТИРОВКИ СПИСКА СЛИЯНИЕМ
int Length(Record* head);
void FrontBackSplit(Record* source, Record** frontRef, Record** backRef);
Record* SortedMerge(Record* a, Record* b);
void MergeSort(Record** headRef);

// Список записей о выдаче сим-карт клиентам
class List
{
	Record* head;
	int count;

public:

	List() : head(nullptr), count(0) {}

	Record** get_head()
	{
		return &head;
	}

	void insert(Record n);
	void returnSim(string sim);
	void load(string filename);
	friend ostream& operator<<(ostream& out, List list);
	void sort()
	{
		MergeSort(&head);
	}
};

