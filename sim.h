#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <limits>

using namespace std;

unsigned int str_hash(string s);
int try_func(int i);

struct Sim
{
	string sim;
	string tarif;
	int year;
	bool exist;

	Sim()
	{
	}
	Sim(string s, string t, int y, bool e)
		: sim(s), tarif(t), year(y), exist(e)
	{
	}

	void load(istream& in);
	void input();
	friend ostream& operator<<(ostream& out, Sim s);
};

template<typename K, typename V>
class HashNode
{
public:

	K key;
	V value;

	HashNode() {}
	HashNode(K key, V value)
	{
		this->value = value;
		this->key = key;
	}
};

template<typename K, typename V>
class HashMap
{
	HashNode<K, V>** arr;
	int capacity;
	int size;

	HashNode<K, V>* dummy; // для того, чтобы помечать элементы удаленными 
	function<unsigned int(K)> hashFunc;
	function<int(int)> tryFunc;

public:

	HashMap(function<unsigned int(K)> myHashFunc, function<int(int)> myTryFunc)
	{
		capacity = 100; // ЕМКОСТЬ ХЭШ ТАБЛИЦЫ!!! 
		size = 0;
		arr = new HashNode<K, V> *[capacity];

		for (int i = 0; i < capacity; i++)
			arr[i] = NULL;

		dummy = new HashNode<K, V>;
		hashFunc = myHashFunc;
		tryFunc = myTryFunc;
	}

	int hashCode(K key)
	{
		return hashFunc(key) % capacity;
	}

	HashNode<K, V>*& find(K key, bool breakOnDummy = false)
	{
		int index;
		int tryCount = 0;
		int hashIndex = hashCode(key);

		do
		{
			index = (hashIndex + tryFunc(tryCount)) % capacity;

			tryCount++;
			if (breakOnDummy && arr[index] == dummy)
				break;

			if (index == hashIndex) // зацикливание поиска 
			{
				hashIndex++; // смещаемся на одну позицию
				hashIndex %= capacity;
			}

		} while (arr[index] != NULL && arr[index]->key != key);  

		cout << "find() -> index = " << index << endl;
		return arr[index];
	}

	void put(K key, V value)
	{
		if (size == capacity)
		{
			throw length_error("Hash::put() size==capacity, no empty slots!");
		}
		HashNode<K, V>*& p = find(key, true);
		if (p == NULL || p == dummy)
			size++;
		p = new HashNode<K, V>(key, value);
	}

	bool remove(K key)
	{
		HashNode<K, V>*& p = find(key);
		if (!p)
			return false;

		p = dummy;
		size--;
		return true;
	}

	V* get(K key)
	{
		HashNode<K, V>*& p = find(key);
		if (!p)
			throw invalid_argument("Hash::get() no such key!");
		return &p->value;
	}

	int sizeofMap()
	{
		return size;
	}

	bool isEmpty()
	{
		return size == 0;
	}

	void display()
	{
		cout << "Hash Table:" << endl;
		for (int i = 0; i < capacity; i++)
		{
			cout << i << "\t";
			if (arr[i] != NULL && arr[i] != dummy)
				cout << "key = " << arr[i]->key
				<< ",  value = " << arr[i]->value << endl;
			else if (arr[i] == NULL)
				cout << "NULL" << endl;
			else
				cout << "DEL" << endl;
		}
		cout << "------------------------" << endl << endl;
	}

	void print()
	{
		for (int i = 0; i < capacity; i++)
		{
			if (arr[i] != NULL && arr[i] != dummy)
				cout << arr[i]->value;
		}
	}
	friend class SimHash;
};

class SimHash
{
	HashMap<string, Sim>* hash;

public:

	SimHash()
	{
		hash = new HashMap<string, Sim>(str_hash, try_func);
	}

	void load(string filename)
	{
		ifstream fin(filename);
		int n;
		fin >> n;
		fin.ignore();

		for (int i = 0; i < n; i++)
		{
			Sim sim;
			sim.load(fin);
			fin.ignore();
			hash->put(sim.sim, sim);
		}
		fin.close();
	}

	void put(Sim s)
	{
		hash->put(s.sim, s);
	}

	Sim* find(string sim)
	{
		try
		{
			Sim* s = hash->get(sim);
			return s;
		}

		catch (invalid_argument e)
		{
			cout << "Нет сим карты с таким номером!" << endl;
			return nullptr;
		}
	}

	void findByTarif(string tarif)
	{
		for (int i = 0; i < hash->capacity; i++)
		{
			if (hash->arr[i] != NULL && hash->arr[i] != hash->dummy)
			{
				Sim s = hash->arr[i]->value;
				if (s.tarif == tarif) 
				{
					cout << s;
				}
			}
		}
	}

	void remove(string sim)
	{
		hash->remove(sim);
	}

	void print()
	{
		cout << endl;
		cout << "Список сим-карт: " << endl;
		hash->print();
	}

	~SimHash()
	{
		delete hash;
	}
};