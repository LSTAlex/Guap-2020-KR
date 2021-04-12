#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace std;

struct Client // структура для представления узлов дерева
{
	string passport;
	string info;
	string fio;
	int year;
	string address;

	unsigned char height;

	Client* left;
	Client* right;

	Client() : left(nullptr), right(nullptr), height(1)
	{
	}


	Client(string passport, string info, string fio, int year, string address) :
		passport(passport), info(info), fio(fio), year(year), address(address),
		left(nullptr), right(nullptr), height(1)
	{
	}

	void load(istream& in);

	void input();

	void print();
};

unsigned char height(Client* p);
void fixheight(Client* p);
int bfactor(Client* p);
Client* rotateright(Client* p); // правый поворот вокруг p
Client* rotateleft(Client* q); // левый поворот вокруг q 
Client* balance(Client* p); // балансировка узла p
Client* insert(Client* p, Client k); // вставка ключа k в дерево с корнем p
void print(Client* root, int level); // печать дерева по уровням
Client* findmin(Client* p); // поиск узла с минимальным ключом в дереве p
Client* removemin(Client* p); // удаление узла с минимальным ключом из дерева p
Client* remove(Client* p, string passport); // удаление ключа k из дерева p
Client* find(Client* p, string passport); // поиск в дереве по паспорту 
void find_by_info(Client* p, string info); // поиск в дереве по части фио или адреса

// АВЛ-дерево клиентов
class AvlTree
{
	Client* root;

public:

	AvlTree() : root(nullptr)
	{
	}

	// печать всех клиентов 
	void print()
	{
		cout << endl;
		cout << "Зарегистрированные клиенты: " << endl;
		::print(root, 0);
		cout << endl << endl;
	}

	// вставка нового клиента в дерево 
	void insert(Client n)
	{
		root = ::insert(root, n);
	}

	// удаление клиента по паспорту 
	void remove(string passport)
	{
		root = ::remove(root, passport);
	}

	// загрузка клиентов из файла 
	void load(string filename)
	{
		ifstream fin(filename);

		int n;

		fin >> n;

		fin.ignore();

		for (int i = 0; i < n; i++)
		{
			Client client;
			client.load(fin);
			fin.ignore();
			insert(client);
		}
		fin.close();
	}

	// поиск клиента по паспорту 
	Client* find(string passport)
	{
		return ::find(root, passport);
	}

	// поиск клиента по части адреса или ФИО 
	void find_by_info(string info)

	{
		::find_by_info(root, info);
	}
};