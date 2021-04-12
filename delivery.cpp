#include "delivery.h"
#include <regex>

// Загрузка записей о выдаче симок из файла
void Record::load(istream& in)
{
	getline(in, passport);
	getline(in, number);
	getline(in, start_date);
	getline(in, finish_date);
	string return_str;
	getline(in, return_str);
	is_returned = return_str == "true" ? true : false;
}

// Добавление новой записи о выдаче сим-ки (ввод данных пользователем)
void Record::input()
{
	istream& in = cin;

	while (true)
	{
		cout << "Паспорт: "; getline(in, passport);
		if (regex_match(passport, regex("\[0-9]{4}-\[0-9]{6}")))
			break;
		else
			cout << "Некорректный формат паспорта!" << endl;
	}

	while (true)
	{
		cout << "Номер симки: "; getline(in, number);
		if (regex_match(number, regex("\[0-9]{3}-\[0-9]{6}")))
			break;
		else
			cout << "Некорректный формат сим-карты!" << endl;
	}
	cout << "Дата выпуска: "; getline(in, start_date);
	cout << "Дата окончания: "; getline(in, finish_date);
	is_returned = false;
}

// печать записи о выдаче симки
ostream& operator<<(ostream& out, Record n) // перегрузка потока вывода
{
	out << n.passport << " " << n.number << " " << n.start_date << " " <<
		n.finish_date << " ";
	if (n.is_returned == true)
		out << "Вернули";
	else
		out << "Выдана";
	out << endl;
	return out;
}

// Список выдачи и возврата SIM-карт пользователям
void List::insert(Record n)
{
	Record* tnode;
	tnode = new Record(n);

	if (tnode != nullptr)
	{
		tnode->next = head;
		count++;
		head = tnode;
	}
}

// возврат сим-карты 
void List::returnSim(string sim)
{
	Record* current = head;

	while (current != nullptr)
	{
		if (current->number == sim)
			current->is_returned = true;
		current = current->next;
	}
	cout << endl;
}

// Загрузка из файла списка выдачи и возврата SIM-карт пользователям
void List::load(string filename)
{
	ifstream fin(filename);
	int n;
	fin >> n;
	fin.ignore();

	for (int i = 0; i < n; i++)
	{
		Record record;
		record.load(fin);
		fin.ignore();
		insert(record);
	}
	fin.close();
}

// Печать списка выдачи и возврата SIM-карт пользователям
ostream& operator<<(ostream& out, List list)
{
	out << "Регистрация Сим-карт: " << endl;
	Record* current = list.head;

	while (current != nullptr)
	{
		cout << *current;
		current = current->next;
	}

	cout << endl;
	return out;
}
//------------------------------------------------------ 

// Функции для сортировки односвязного списка слиянием
int Length(Record* head)
{
	int count = 0;
	Record* current = head;

	while (current != nullptr)
	{
		count++;
		current = current->next;
	}

	return(count);
}

void FrontBackSplit(Record* source, Record** frontRef, Record** backRef)
{
	int len = Length(source);
	int i;
	Record* current = source;

	if (len < 2)
	{
		*frontRef = source;
		*backRef = nullptr;
	}
	else
	{
		int hopCount = (len - 1) / 2;

		for (i = 0; i < hopCount; i++)
		{
			current = current->next;
		}
		// исходный список разбивается на два подсписка 
		*frontRef = source;
		*backRef = current->next;
		current->next = nullptr;
	}
}

Record* SortedMerge(Record* a, Record* b)
{
	Record* result = nullptr;

	if (a == nullptr) return(b);

	else if (b == nullptr) return(a);

	if (a->number <= b->number)
	{
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else
	{
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return(result);
}

void MergeSort(Record** headRef)
{
	Record* head = *headRef;
	Record* a;
	Record* b;

	// вырожденный случай – длина списка равно 0 или 1
	if ((head == nullptr) || (head->next == nullptr))
	{
		return;
	}

	FrontBackSplit(head, &a, &b);
	MergeSort(&a); // рекурсивная сортировка подсписков 
	MergeSort(&b);
	*headRef = SortedMerge(a, b);
}
//------------------------------------------------------ 