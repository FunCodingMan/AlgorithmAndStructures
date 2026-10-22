#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <locale.h>

const int INF = 1e9;
const int FIRST_CAPITAL_NUMBER = 1;
const int SECOND_CAPITAL_NUMBER = 2;

using namespace std;

struct Road {
    int from, to;   
    int length;
    int time;
};

void readRoads(vector<Road>& roads, vector<vector<int>>& distLength, vector<vector<int>>& distTime, int countRoads, istream& in){
    int from, to, length, time;
    for (int i = 0; i < countRoads; ++i)
    {
        in >> from >> to >> length >> time;
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
        for (int i = 1; i <= count; ++i)
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
    for (int i = 1; i <= countCities; ++i)
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

int ValidateRoads(const vector<Road>& roads, vector<vector<bool>>& isValidRoad, const vector<vector<int>>& distLength, const vector<vector<int>>& distTime, int lengthLimit, int timeLimit)
{
    int countValidRoads = 0;
    for (const Road& road : roads)
    {
        if (IsRoadValid(road, distLength, distTime, lengthLimit, timeLimit))
        {
            int u = min(road.from, road.to);
            int v = max(road.from, road.to);
            isValidRoad[u][v] = true;
            countValidRoads++;
        }
    }
    return countValidRoads;
}

void PrintResults(const vector<int>& validCities, const vector<vector<bool>>& isValidRoad, int countCities, int validRoadsCount)
{
    int validCitiesCount = validCities.size();

    if (validCitiesCount == 0 || validRoadsCount == 0)
    {
        cout << "No\n";
        return;
    }

    cout << validCitiesCount << endl;
    cout << validCities[0];
    for (size_t i = 1; i < validCitiesCount; ++i)
    {
        cout << " " << validCities[i];
    }
    cout << endl;

    cout << validRoadsCount << endl;
    for (int i = 1; i <= countCities; ++i)
    {
        for (int j = i + 1; j <= countCities; ++j)
        {
            if (isValidRoad[i][j])
            {
                cout << i << " " << j << endl;
            }
        }
    }
}

void Loop()
{
    string filename;
    cout << "=== Система анализа транспортной сети (Задача 32) ===" << endl;

    while (true)
    {
        cout << "\nВведите имя файла с данными (или 'exit' для выхода): ";
        cin >> filename;

        if (filename == "exit" || filename == "quit" || filename == "q")
        {
            cout << "Завершение работы программы." << endl;
            break;
        }

        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "Ошибка: не удалось открыть файл '" << filename << "'. Проверьте путь и попробуйте снова." << endl;
            continue;
        }

        int countCities, countRoads, countValidRoads, lengthLimit, timeLimit;
        if (!(fin >> countCities >> countRoads >> lengthLimit >> timeLimit))
        {
            cout << "Ошибка: неверный формат данных в файле." << endl;
            continue;
        }

        vector<Road> roads;
        vector<vector<int>> distLength(countCities + 1, vector<int>(countCities + 1, INF));
        vector<vector<int>> distTime(countCities + 1, vector<int>(countCities + 1, INF));
        vector<vector<bool>> isValidRoad(countCities + 1, vector<bool>(countCities + 1, false));

        InitializeMatrix(countCities, distLength, distTime);
        readRoads(roads, distLength, distTime, countRoads, fin); // Читаем из файлового потока

        RunFloyd(countCities, distLength);
        RunFloyd(countCities, distTime);

        vector<int> validCities;
        ValidateCities(validCities, distLength, distTime, lengthLimit, timeLimit, countCities);

        countValidRoads = ValidateRoads(roads, isValidRoad, distLength, distTime, lengthLimit, timeLimit);

        cout << "\n--- Результат расчёта (файл: " << filename << ") ---" << endl;
        PrintResults(validCities, isValidRoad, countCities, countValidRoads);
        cout << "-----------------------------------------------" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.CP1251");
    Loop();
    return 0;
}