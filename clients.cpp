#include <regex>
#include "clients.h"
// считываение клиента из потока ввода (из файла)
void Client::load(istream& in)
{
	getline(in, passport);
	getline(in, info);
	getline(in, fio);
	string year_str;
	in >> year;
	in.ignore();
	getline(in, address);
}

// ввод данных нового клиента
void Client::input()
{
	istream& in = cin;
	in.ignore();

	while (true)
	{
		cout << "Паспорт: "; getline(in, passport);
		if (regex_match(passport, regex("\[0-9]{4}-\[0-9]{6}")))
			break;
		else
			cout << "Некорректный формат паспорта NNNN-NNNNNN!" << endl;
	}

	cout << "Кем и когда выдан: ";  getline(in, info);
	cout << "ФИО: "; getline(in, fio);
	cout << "Год рождения: "; string year_str;
	in >> year;
	in.ignore();
	cout << "Aдрес: "; getline(in, address);
}

// печать информации о клиенте
void Client::print()
{
	cout << passport << " " << info << " " << fio << " " << year << " " << address <<
		endl;
}

//  высота дерева
unsigned char height(Client* p)
{
	return p ? p->height : 0;
}

// пересчет высоты дерева
void fixheight(Client* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);

	p->height = (hl > hr ? hl : hr) + 1;
}

// b-фактор узла (разность высот правого и левого поддерева)
int bfactor(Client* p)
{
	return height(p->right) - height(p->left);
}

Client* rotateright(Client* p) // правый поворот вокруг p 
{
	Client* q = p->left;

	p->left = q->right;

	q->right = p;

	fixheight(p);

	fixheight(q);

	return q;
}

Client* rotateleft(Client* q) // левый поворот вокруг q
{
	Client* p = q->right;

	q->right = p->left;

	p->left = q;

	fixheight(q);

	fixheight(p);

	return p;
}

Client* balance(Client* p) // балансировка узла p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}

	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}

	return p; // балансировка не нужна 
}

Client* insert(Client* p, Client k) // вставка ключа k в дерево с корнем p
{

	if (!p) return new Client(k);

	if (k.passport < p->passport)
		p->left = insert(p->left, k);
	else
		p->right = insert(p->right, k);
	return balance(p);
}


void print(Client* root, int level = 0)
{
	if (root == nullptr)
		return;

	print(root->right, level + 1);

	// for(int i=0; i<level; i++)   
	cout << "  ";

	root->print(); // cout << endl; 
	print(root->left, level + 1);
}

Client* findmin(Client* p) // поиск узла с минимальным ключом в дереве p
{
	return p->left ? findmin(p->left) : p;
}

Client* removemin(Client* p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)
		return p->right;
	p->left = removemin(p->left);
	return balance(p);
}

Client* remove(Client* p, string passport) // удаление ключа k из дерева p
{
	if (!p) return 0;

	if (passport < p->passport)
		p->left = remove(p->left, passport);
	else if (passport > p->passport)
		p->right = remove(p->right, passport);
	else
	{
		Client* q = p->left;
		Client* r = p->right;

		delete p;

		if (!r) return q;
		Client* min = findmin(r);
		min->right = removemin(r);
		min->left = q;
		return balance(min);
	}

	return balance(p);
}

// поиск клиента по номеру паспорта (вернет указатель на узел дерева)
Client* find(Client* p, string passport)
{
	if (!p) return 0;
	if (passport == p->passport) return p;
	if (passport < p->passport)

	return find(p->left, passport);
	return find(p->right, passport);
}

// поиск клиента по части адреса или ФИО
void find_by_info(Client* p, string info)
{
	if (!p) return;
	if (p->fio.find(info) != string::npos) 
	{
		p->print();
	}

	if (p->address.find(info) != string::npos) 
	{
		p->print();

	}

	find_by_info(p->left, info);
	find_by_info(p->right, info);
}
