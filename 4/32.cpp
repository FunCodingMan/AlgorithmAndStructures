/*
Условие:   32. В некотором государстве имеется N городов, пронумерованных
от 1 до N, и M  дорог,  связывающих пары городов  (2 <= N <= 30, 
M >= 1). По каждой дороге разрешено движение в обоих направлениях. 
Между двумя городами не может быть более одной дороги.  Столице
присвоен номер 1. Президент часто ездит во второй по значимости
город государства, который часто называется  второй  столицей и
имеет номер 2. В целях  безопасности для каждой  поездки  между 
столицами  прокладывается  новый   маршрут.  В  зависимости  от
обстоятельств вводится ограничение на длину  маршрута  либо  на
время в пути. Служба  безопасности уточнила длину каждой дороги
и определила время, необходимое для проезда по ней. Было решено
составить карту, на которой имеются  все  допустимые  города  и 
дороги, но нет ничего лишнего. Допустимый город или дорога должны
удовлетворять следующим требованиям:
1) имеется хотя бы один путь между столицами,  включающий  этот
город  или  дорогу,  длина  которого  не  более  S   километров 
(1 <= S <= 10^4);
2) имеется хотя бы один путь между столицами,  включающий  этот
город или дорогу, по которому можно проехать не  более  чем  за 
T минут (1 <= T <= 10^4).
Пути, удовлетворяющие каждому из этих  двух  требований,  могут 
быть разными. В своих поездках президент иногда имеет намерение
посещать и  другие  города,  находящиеся  на  тупиковых  путях, 
поэтому маршрут может  иметь  повторяющиеся  города  и  дороги.
Ввод. Первая строка содержит целые положительные числа  N, M, S
и T через пробел. Далее  следуют  M  строк  описания  дорог  по 
четыре  целых  положительных   числа   A, B, C и D   в   строке 
(1 <= A < B <= N, 1 <= C, D <= 10^4). Первые два  числа  задают
номера  городов,  третье – длину  дороги,  а  четвертое – время 
проезда по ней.
Вывод. В первой строке выводится количество оставшихся  городов. 
Во второй строке выводятся через пробел в  порядке  возрастания 
номера оставшихся городов. В  третьей  строке  находится  число 
оставшихся дорог K. Каждая из следующих K  строк  содержит  два
целых положительных  числа  Ui  и  Vi  (Ui < Vi),  определяющих 
очередную дорогу. Список дорог выводится в порядке  возрастания 
Ui,  а  при  равных  значениях Ui  - по  возрастанию  Vi.  Если 
допустимых путей нет, вывести No (13).
Примеры
Ввод 1     Ввод 2
5 5 9 12   4 6 8 12
1 2 7 5    1 2 6 20
2 4 2 6    2 4 5 6
1 4 4 4    1 4 4 4
3 4 1 3    3 4 7 3
1 3 2 5    1 3 3 1
           2 3 6 5
Вывод 1    Вывод 2
3          No
1 2 4
3
1 2
1 4
2 4
Пояснение. В первом примере город 5 не связан дорогами с другими 
городами, а допустимых путей через город 3 нет. Во втором примере
нет допустимых по длине путей через вершины 3 и 4, а прямой путь 
1 – 2 недопустим по времени.


Автор: Камалов Данил Маратович
Среда выполнения: Visual Studio Code, g++
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

const int INF = 1e9;
const int FIRST_CAPITAL_NUMBER = 1;
const int SECOND_CAPITAL_NUMBER = 2;

using namespace std;

struct Road {
    int from, to;
    int length;
    int time;
};

void readRoads(vector<Road>& roads, vector<vector<int>>& distLength, vector<vector<int>>& distTime, int countRoads)
{
    int from, to, length, time;
    for (int i = 0; i < countRoads; ++i)
    {
        cin >> from >> to >> length >> time;
        roads.push_back({from, to, length, time});
        distLength[from][to] = length;
        distLength[to][from] = length;
        distTime[from][to] = time;
        distTime[to][from] = time;
    }
}
void InitializeMatrix(int countCities, vector<vector<int>>& distLength, vector<vector<int>>& distTime)
{

    for (int i = 1; i <= countCities; ++i)
    {
        distLength[i][i] = 0;
        distTime[i][i] = 0;
    }
}

void RunFloyd(int count, vector<vector<int>>& dist)
{
    for (int k = 1; k <= count; ++k)
    {
        for (int i = 1; i <= count; ++ i)
        {
            for (int j = 1; j <= count; ++j)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

bool IsCityValid(int length, int time, int lengthLimit, int timeLimit)
{
    return length <= lengthLimit && time <= timeLimit;
}

void ValidateCities(vector<int>& validCities, const vector<vector<int>>& distLength, const vector<vector<int>>& distTime, int lengthLimit, int timeLimit, int countCities)
{
    for (int i = 1; i < countCities; ++i)
    {
        if (IsCityValid(
            distLength[FIRST_CAPITAL_NUMBER][i] + distLength[i][SECOND_CAPITAL_NUMBER],
            distTime[FIRST_CAPITAL_NUMBER][i] + distTime[i][SECOND_CAPITAL_NUMBER],
            lengthLimit,
            timeLimit
        ))
        {
            validCities.push_back(i);
        }
    }
}

bool IsRoadValid(const Road& road, const vector<vector<int>>& distLength, const vector<vector<int>>& distTime, int lengthLimit, int timeLimit) 
{
    int from = road.from;
    int to = road.to;
    int length = road.length;
    int time = road.time;
    
    bool lengthIsOk = (distLength[1][from] + length + distLength[to][2] <= lengthLimit) ||
                      (distLength[1][to] + length + distLength[from][2] <= lengthLimit);
    bool timeIsOk = (distTime[1][from] + time + distTime[to][2] <= timeLimit) ||
                    (distTime[1][to] + time + distTime[from][2] <= timeLimit);

    return lengthIsOk && timeIsOk;
}

void ValidateRoads(const vector<Road>& roads, set<pair<int, int>>& validRoads, const vector<vector<int>>& distLength, const vector<vector<int>>& distTime, int lengthLimit, int timeLimit, int countCities)
{
    for (Road road: roads)
    {
        if (IsRoadValid(road, distLength, distTime, lengthLimit, timeLimit))
        {
            int first = min(road.from, road.to);
            int second =  max(road.from, road.to);
            validRoads.insert({first, second});
        }
    } 
}

void PrintResults(const vector<int>& validCities, const set<pair<int, int>>& validRoads)
{
    int validCitiesCount = validCities.size();
    int validRoadsCount = validRoads.size();
    if (validCitiesCount == 0 || validRoadsCount == 0)
    {
        cout << "No\n";
        return;
    }

    cout << validCitiesCount << endl;

    cout << validCities[0];
    for (int i = 1; i < validCitiesCount; ++i)
    {
        cout << " " << validCities[i];
    }
    cout << endl;


    cout << validRoadsCount << endl;

    for (const auto& road : validRoads)
    {
        cout << road.first << " " << road.second << endl;
    }
}

void Loop()
{
    int countCities, countRoads, lengthLimit, timeLimit;
    cin >> countCities >> countRoads >> lengthLimit >> timeLimit;
    
    vector<Road> roads;
    vector<vector<int>> distLength(countCities + 1, vector<int>(countCities + 1, INF));
    vector<vector<int>> distTime(countCities + 1, vector<int>(countCities + 1, INF));
    
    InitializeMatrix(countCities, distLength, distTime);

    readRoads(roads, distLength, distTime, countRoads);

    RunFloyd(countCities, distLength);
    RunFloyd(countCities, distTime);

    vector<int> validCities;
    ValidateCities(validCities, distLength, distTime, lengthLimit, timeLimit, countCities);

    set<pair<int, int>> validRoads;

    ValidateRoads(roads, validRoads, distLength, distTime, lengthLimit, timeLimit, countCities);

    PrintResults(validCities, validRoads);
}

int main()
{
    Loop();
}