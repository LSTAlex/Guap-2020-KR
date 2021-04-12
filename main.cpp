#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#include <windows.h>

#include "clients.h"
#include "sim.h"
#include "delivery.h"

using namespace std;

int menu()
{
	vector<string> vars =
	{
	"выход",
	"регистрация нового клиента;",
	"снятие с обслуживания клиента;",
	"просмотр всех зарегистрированных клиентов;",
	"поиск клиента по «№ паспорта»;",
	"поиск клиента по фрагментам ФИО или адреса;",
	"добавление новой SIM-карты;",
	"удаление сведений о SIM-карте;",
	"просмотр всех имеющихся SIM-карт;",
	"поиск SIM-карты по «№ SIM-карты»;",
	"поиск SIM-карты по тарифу;",
	"регистрация выдачи клиенту SIM-карты;",
	"регистрация возврата SIM-карты от клиента",
	"печать списка регистрации SIM-карт"
	};
	cout << endl;
	cout << "МЕНЮ:" << endl;

	for (int i = 0; i < vars.size(); i++)
	{
		cout << i << ") " << vars[i] << endl;
	}

	while (true)
	{
		cout << "Выберите действие: ";
		int v;
		cin >> v;
		if (v < 0 || v >= vars.size())
			cout << "Неправильный индекс действия" << endl;
		else
			return v;
	}
}

int main()
{
	setlocale(LC_CTYPE, "");
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода 
	AvlTree clients;
	clients.load("clients.txt");
	SimHash sims;
	sims.load("sim.txt");
	List delivery;
	delivery.load("delivery.txt");
	delivery.sort();

	int action = menu();

	while (action)
	{
		switch (action)
		{
		case 1: // "регистрация нового клиента;" 
		{
			Client c;
			c.input();
			clients.insert(c);
		}
		break;
		case 2: // "снятие с обслуживания клиента;" 
		{
			string passport;
			cout << "Введите паспорт клиента для снятия с обслуживания: ";
			cin >> passport;
			Client* c = clients.find(passport);

			if (!c)
			{
				cout << "Нет клиента с таким паспортом" << endl;
			}
			else
				clients.remove(passport);
		}
		break;
		case 3: // "просмотр всех зарегистрированных клиентов;"
			clients.print();
			break;
		case 4: // "поиск клиента по «№ паспорта»;" 
		{
			string passport;
			cout << "Введите паспорт клиента: ";
			cin >> passport;
			Client* c = clients.find(passport);

			if (!c)
			{
				cout << "Нет клиента с таким паспортом" << endl;
			}
			else
				c->print();
		}
		break;
		case 5: // "поиск клиента по фрагментам ФИО или адреса;" 
		{
			string info;
			cout << "Bведите фрагмент ФИО или адреса для поиска: ";
			cin >> info;
			clients.find_by_info(info);
		}
		break;
		case 6: // "добавление новой SIM-карты;" 
		{
			Sim s;
			cin.ignore();
			s.input();
			sims.put(s);
		}
		break;
		case 7: // "удаление сведений о SIM-карте;", 
		{
			string sim;
			cout << "Введите номер сим-карты для удаления: ";
			cin >> sim;
			sims.remove(sim);
		}
		break;
		case 8: // "просмотр всех имеющихся SIM-карт;" 
			sims.print();
			break;
		case 9: // "поиск SIM-карты по «№ SIM-карты»;" 
		{
			string sim;
			cout << "Введите номер сим-карты для удаления: ";
			cin >> sim;

			Sim* s = sims.find(sim);
			cout << *s << endl;
		}
		break;
		case 10: // "поиск SIM-карты по тарифу;"
		{
			string tarif;
			cout << "Введите тариф для поиска: ";
			cin >> tarif;
			sims.findByTarif(tarif);
		}
		break;
		case 11: // "регистрация выдачи клиенту SIM-карты;"
		{
			Record r;
			cin.ignore();
			r.input();

			Sim* s = sims.find(r.number);

			if (!s) break;
			if (s->exist) // если sim с таким номером в наличии
			{
				delivery.insert(r); // регистрация выдачи сим-карты 
				s->exist = false; // пометили, как отсутствующую 
			}
			else
				cout << "У нас нет сим-карты с таким номером" << endl;
		}
		break;
		case 12: // "регистрация возврата SIM-карты от клиента" 
		{
			string sim;
			cout << "Введите номер сим-карты для возврата: ";
			cin >> sim;

			delivery.returnSim(sim);
			// Отмечаем, что теперь эта симка в наличии 
			Sim* s = sims.find(sim);
			s->exist = true;
		}
		break;
		case 13: // "печать списка регистрации SIM-карт"
			delivery.sort();
			cout << endl << delivery;
			break;
		}
		action = menu();
	}
	cout << "Bye!" << endl;
}
