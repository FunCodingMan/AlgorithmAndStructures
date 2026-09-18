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

#include "MyStack.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

const int NUMBER_OF_VARIABLES = 26;

struct Expression
{
	double val;
	string formula;
	int priority;
};

struct Variable
{
	double val;
	bool isExist;
};

bool IsVariable(const string& token)
{
	return token.length() == 1 && token[0] >= 'a' && token[0] <= 'z';
}

bool IsBinaryOperation(const string& token)
{
	return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

bool IsUnaryOperation(const string& token)
{
	return token == "~" || token == "SIN" || token == "COS" || token == "EXP";
}

int GetPriority(const string& operation)
{
	if (operation == "+" || operation == "-")
		return 1;
	if (operation == "*" || operation == "/")
		return 2;
	if (operation == "^")
		return 3;
	if (operation == "~")
		return 4;
	if (operation == "SIN" || operation == "COS" || operation == "EXP")
		return 5;
	return 6;
}

vector<string> ParseTokens(string expression)
{
	vector<string> tokens;
	string temp = "";
	for (char ch : expression)
	{
		if (ch == ' ' && !temp.empty())
		{
			tokens.push_back(temp);
			cout << temp << "\n";
			temp.clear();
		}
		else
		{
			temp += ch;
		}
	}
	return tokens;
}

void ProcessVariable(const string& token, Variable vars[], MyStack<Expression>& stack)
{
	int idx = token[0] - 'a';
	if (!vars[idx].isExist)
	{
		cout << "Введите значение переменной '" << token << "': ";
		cin >> vars[idx].val;
		vars[idx].isExist = true;
	}

	Expression exp;
	exp.val = vars[idx].val;
	exp.formula = token;
	exp.priority = 6;

	stack.push(exp);
}

void ProcessBinaryOperation(const string& operation, MyStack<Expression>& stack)
{
	Expression right = stack.pop();
	Expression left = stack.pop();

	double resVal = 0;
	int curPriority = GetPriority(operation);

	if (operation == "+")
		resVal = left.val + right.val;
	else if (operation == "-")
		resVal = left.val - right.val;
	else if (operation == "*")
		resVal = left.val * right.val;
	else if (operation == "/")
	{
		if (right.val == 0)
			throw runtime_error("Деление на ноль!");
		resVal = left.val / right.val;
	}
	else if (operation == "^")
		resVal = pow(left.val, right.val);

	if (left.priority < curPriority || (left.priority == curPriority && operation == "^"))
	{
		left.formula = "(" + left.formula + ")";
	}
	if (right.priority < curPriority || (right.priority == curPriority && (operation == "-" || operation == "/")))
	{
		right.formula = "(" + right.formula + ")";
	}

	Expression res;
	res.val = resVal;
	res.formula = left.formula + " " + operation + " " + right.formula;
	res.priority = curPriority;

	stack.push(res);
}

void ProcessUnaryOperation(const string& operation, MyStack<Expression>& stack)
{
	Expression expr = stack.pop();

	double resVal = 0;
	string resFormula = "";
	int curPriority = GetPriority(operation);

	if (operation == "~")
	{
		resVal = -expr.val;
		if (expr.priority < 4)
		{
			expr.formula = "(" + expr.formula + ")";
		}
		resFormula = "~" + expr.formula;
	}
	else if (operation == "SIN")
	{
		resVal = sin(expr.val);
		resFormula = "SIN(" + expr.formula + ")";
	}
	else if (operation == "COS")
	{
		resVal = cos(expr.val);
		resFormula = "COS(" + expr.formula + ")";
	}
	else if (operation == "EXP")
	{
		resVal = exp(expr.val);
		resFormula = "EXP(" + expr.formula + ")";
	}

	Expression res;
	res.val = resVal;
	res.formula = resFormula;
	res.priority = curPriority;

	stack.push(res);
}

void ProcessToken(const string& token, Variable vars[], MyStack<Expression>& stack)
{
	if (IsVariable(token))
	{
		ProcessVariable(token, vars, stack);
	}
	else if (IsBinaryOperation(token))
	{
		ProcessBinaryOperation(token, stack);
	}
	else if (IsUnaryOperation(token))
	{
		ProcessUnaryOperation(token, stack);
	}
	else
	{
		throw runtime_error("Неизвестный символ: '" + token + "'");
	}
}

void Initialize(Variable vars[])
{
	for (int i = 0; i < NUMBER_OF_VARIABLES; ++i)
	{
		vars[i].val = 0.0;
		vars[i].isExist = false;
	}
}

void PrintFinalResult(MyStack<Expression>& stack)
{
	Expression res = stack.pop();
	if (!stack.isEmpty())
	{
		cout << "Ошибка: в файле некорректное выражение! Остались лишние операнды!\n";
		return;
	}
	cout << "\n-------------------------------\n";
	cout << "Инфиксная форма: " << res.formula << endl;
	cout << "Результат: " << res.val << endl;
	cout << "-------------------------------\n";
}

void ProcessFile(const string& filename, Variable vars[])
{
	ifstream file(filename);
	if (!file.is_open())
	{
		cout << "Ошибка: не удалось открыть файл " << filename << endl;
		return;
	}

	Initialize(vars);

	MyStack<Expression> stack;
	string token;
	bool hasData = false;

	cout << "\nЧтение файла...\n";
	try
	{
		while (file >> token)
		{
			hasData = true;
			ProcessToken(token, vars, stack);
		}

		if (!hasData)
		{
			cout << "Файл пуст!\n";
		}
		else
		{
			PrintFinalResult(stack);
		}
	}
	catch (const exception& e)
	{
		cout << "Ошибка выражения: " << e.what() << endl;
	}

	file.close();
}

void Loop()
{
	int choice = -1;
	string filename;
	Variable vars[NUMBER_OF_VARIABLES];

	cout << "Лабораторная работа 2. Задание 24.\n";
	do
	{
		cout << "Меню:\n";
		cout << "1. Обработать выражение из файла\n";
		cout << "0. Выход из программы\n";
		cout << "Ваш выбор: ";

		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "Введите имя файла: ";
			cin >> filename;
			ProcessFile(filename, vars);
			break;
		case 0:
			cout << "Завершение работы\n";
			break;
		default:
			cout << "Неверный пункт меню! Попробуйте снова.\n";
		}
	} while (choice != 0);
}

int main()
{
	Loop();
	return 0;
}
