#include <iostream> 
#include <fstream>
#include <stdexcept>
#include <functional>
#include <limits>
#include <regex>

#include "sim.h"

void Sim::load(istream& in)
{
	getline(in, sim);
	getline(in, tarif);
	in >> year;
	in.ignore();

	string exists_str;
	getline(in, exists_str);
	exist = exists_str == "true" ? true : false;
}

void Sim::input()
{
	istream& in = cin;

	while (true)
	{
		cout << "Номер симки: "; getline(in, sim);
		if (regex_match(sim, regex("\[0-9]{3}-\[0-9]{7}")))
			break;
		else
			cout << "Некорректный формат сим-карты NNN-NNNNNNN!" << endl;
	}

	cout << "Тариф: "; getline(in, tarif);
	cout << "Год выпуска: "; in >> year;
	in.ignore();
	exist = true;
}

ostream& operator<<(ostream& out, Sim s)
{
	out << s.sim << " " << s.tarif << " " << s.year << " " << " ";
	if (s.exist)
		out << "В наличии";
	else
		out << "Нет в наличии";
	out << endl;

	return out;
}

unsigned int str_hash(string s)
{
	const int p = 31;
	long long hash = 0, p_pow = 1;

	for (size_t i = 0; i < s.length(); ++i)
	{
		hash += (s[i] - 'a' + 1) * p_pow;
		p_pow *= p;
	}

	return hash;
}

int try_func(int i)
{
	return i + i * i;
}