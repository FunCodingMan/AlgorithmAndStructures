/*
Условие: Имеется  выражение  в  постфиксной  форме,   включающее 
операции '+', '-', '*', '/', '^', ~ (одноместный минус),  SIN, 
COS, EXP.  Переменные  заданы одной строчной латинской буквой.
Запросить значения переменных и вычислить выражение. Представить
его в инфиксной форме со скобками. Лишние скобки присутствовать
не должны (11).

Автор: Камалов Данил Маратович

Среда выполнения: Visual Studio Code, g++

Источники информации: https://coddy.tech/docs/ru/cpp
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Expression
{
	int val;
	string formula;
};

vector<string> parseTokens(string expression)
{
	vector<string> tokens;
	string temp = "";
	for (char ch: expression)
	{
		if (ch == ' ' && !temp.empty())
		{	
			tokens.push_back(temp);
			cout << temp << "\n";
			temp.clear();
		} else
		{
			temp += ch;
		}
	}
	return tokens;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cerr << "Error: missing parametres!\n";
		cerr << "Use: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

	if (freopen(argv[1], "r", stdin) == nullptr)
	{
		cerr << "Error: failed to open the file '" << argv[1] << "'!\n";
		return 1;
	}

	if (freopen(argv[2], "w", stdout) == nullptr)
	{
		cerr << "Error: failed to open/create the file '" << argv[2] << "'!\n";
		return 1;
	}

	string expression;

	if (getline(cin, expression))
	{
		vector<string> tokens = parseTokens(expression);
	}
	
	return 0;
}
