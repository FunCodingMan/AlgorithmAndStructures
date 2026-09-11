/*
Условие: На вход подаются сведения о сдаче экзаменов учениками 9-х
классов некоторой средней школы. В первой строке сообщается
количество учеников N, которое не меньше 10, но не превосходит
100, каждая из следующих N строк имеет следующий формат:
<Фамилия> <Имя> <оценки>, где <Фамилия> – строка, состоящая не
более чем из 20 символов, <Имя> – строка, состоящая не более чем
из 15 символов <оценки> – через пробел три целых числа,
соответствующие оценкам по пятибалльной системе. <Фамилия> и
<Имя>, а также <Имя> и <оценки> разделены одним пробелом.
Пример входной строки: Иванов Петр 4 5 4
Требуется написать программу, которая будет выводить на экран
фамилии и имена трех лучших по среднему баллу учеников. Если
среди остальных есть ученики, набравшие тот же средний балл,
что и один из трех лучших, то следует вывести и их фамилии и
имена. Требуемые имена и фамилии можно выводить в произвольном
порядке (8).

Автор: Камалов Данил Маратович

Среда выполнения: Visual Studio Code, g++

Источники информации: https://coddy.tech/docs/ru/cpp
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

	int count;

	if (cin >> count)
	{
		vector<Pupil> pupils;

		readPupils(pupils, count);
		sortPupils(pupils);
		printTopPupils(pupils);
	}

	return 0;
}
