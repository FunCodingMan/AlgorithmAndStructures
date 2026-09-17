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
#include <stack>

using namespace std;

const int NUMBER_OF_VARIABLES = 26;

struct Expression
{
	int val;
	string formula;
};

struct Variable
{
	int val;
	bool isExist;
};

vector<string> ParseTokens(string expression)
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

void Initialize(Variable vars[NUMBER_OF_VARIABLES])
{
	for (int i = 0; i <= NUMBER_OF_VARIABLES; ++i)
	{
		vars[i].val = 0.0;
		vars[i].isExist = false;
	}
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

	stack<int> numbers;
	stack<Expression> expressions;
	Variable vars[NUMBER_OF_VARIABLES];


	string expression;

	if (getline(cin, expression))
	{
		vector<string> tokens = ParseTokens(expression);
	}
	
	return 0;
}
