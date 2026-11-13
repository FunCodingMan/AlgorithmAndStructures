/*
Условие:
20. Имеются города с номерами от 1 до N и дороги между некоторыми 
из них. По дороге можно ехать в любом направлении. Известно, какой 
максимальный груз можно провезти по каждой из дорог. Нужно узнать, 
какие максимальные грузы можно доставить из города 1 в остальные 
города. Использовать при решении  бинарную кучу.  Запрещается 
использование приоритетной очереди из C++.
ВВОД из файла INPUT.TXT. Первая строка содержит количество городов N 
и дорог M через пробел. В каждой из следующих M строк находится по 3
числа. Первые два из них - i и j- задают дорогу, а третье Cij – ее
грузоподъемность.
ВЫВОД в файл OUTPUT.TXT. В i-й строке выводится значение максимального 
груза, который может быть доставлен из города 1 до i+1-го города. 
Таким образом, файл OUTPUT.TXT состоит из N-1 строки. Если в какой-либо 
город пути нет, в соответствующей строке вывести -1.
Ограничения: 2 <= N <= 5*10^4; 2 <= M <= 5*10^5; 0 <= Cij <= 5*10^5 (11).
Пример
Ввод
5 6
1 2 6
1 5 7
2 4 3
5 4 1
2 3 6
4 3 1
Вывод
6
6
3
7


Автор: Камалов Данил Маратович
Среда выполнения: Visual Studio Code, g++
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <locale.h>

using namespace std;

const int INF = 1e9;
const int UNREACHABLE = -1;
const int START_CITY = 1;
const string DEFAULT_OUTPUT_FILE = "OUTPUT.TXT";


struct Road {
    int from, to;   
    int weightCapacity;
};

struct HeapNode {
    int city;
    int weightCapacity;
};  

struct Heap {
    vector<HeapNode> nodes;
    vector<int> position;
};

void InitHeap(Heap& heap, int countCities)
{
    heap.nodes.clear();
    heap.position.assign(countCities + 1, -1);
}

bool IsHeapEmpty(const Heap& heap)
{
    return heap.nodes.empty();
}

void SwapHeapNodes(Heap& heap, int firstIdx, int secondIdx)
{
    swap(heap.nodes[firstIdx], heap.nodes[secondIdx]);
    heap.position[heap.nodes[firstIdx].city] = secondIdx;
    heap.position[heap.nodes[secondIdx].city] = firstIdx;
}

void SiftUp(Heap& heap, int idx)
{
    while (idx > 0)
    {
        int parentIdx = (idx - 1) / 2;

        if (heap.nodes[idx].weightCapacity <= heap.nodes[parentIdx].weightCapacity)
        {
            return;
        }

        SwapHeapNodes(heap, idx, parentIdx);
        idx = parentIdx;
    }
}

void SiftDown(Heap& heap, int idx)
{
    size_t size = heap.nodes.size();
    while (true)
    {
        int leftChild = idx * 2 + 1;
        int rightChild = idx * 2 + 2;
        int largest = idx;

        if (leftChild < size && heap.nodes[leftChild].weightCapacity > heap.nodes[largest].weightCapacity)
        {
            largest = leftChild;
        }

        if (rightChild < size && heap.nodes[rightChild].weightCapacity > heap.nodes[largest].weightCapacity)
        {
            largest = rightChild;
        }

        if (largest == idx)
        {
            return;
        }

        SwapHeapNodes(heap, largest, idx);
        idx = largest;
    }
}

void PushOrUpdateHeap(Heap& heap, int city, int weightCapacity)
{
    int currentPos = heap.position[city];
    if (currentPos == -1)
    {
        int newIdx = heap.nodes.size();
        heap.nodes.push_back({city, weightCapacity});
        heap.position[city] = newIdx;
        SiftUp(heap, newIdx);
    }   
    else
    {
        heap.nodes[currentPos].weightCapacity = weightCapacity;
        SiftUp(heap, currentPos);
    }
}

HeapNode PopHeapMax(Heap& heap)
{
    HeapNode maxNode = heap.nodes[0];

    heap.position[maxNode.city] = -1;

    int lastIdx = heap.nodes.size() - 1;

    if (lastIdx > 0)
    {
        heap.nodes[0] = heap.nodes[lastIdx];
        heap.position[heap.nodes[0].city] = 0;
    }

    heap.nodes.pop_back();

    if (!IsHeapEmpty(heap)) {
        SiftDown(heap, 0);
    }

    return maxNode;
}


void ReadRoads(vector<vector<Road>>& roads, int countRoads, istream& in)
{
    int from, to, weightCapacity;
    for (int i = 0; i < countRoads; ++i)
    {
        in >> from >> to >> weightCapacity;
        roads[from].push_back({to, weightCapacity});
        roads[to].push_back({from, weightCapacity});
    }
}

void FindMaxWeightCapacity(int countCities, const vector<vector<Road>>& roads, vector<int>& maxWeightCapacity)
{
    maxWeightCapacity.assign(countCities + 1, UNREACHABLE);
    maxWeightCapacity[START_CITY] = INF;

    Heap heap;
    InitHeap(heap, countCities);
    PushOrUpdateHeap(heap, START_CITY, INF);

    while (!IsHeapEmpty(heap))
    {
        HeapNode curNode = PopHeapMax(heap);
        int u = curNode.city;
        int curMaxWeightCapacity = curNode.weightCapacity;

        for (const Road& road: roads[u])
        {
            int v = road.to;
            int availableWeightCapacity = min(curMaxWeightCapacity, road.weightCapacity);

            if (availableWeightCapacity > maxWeightCapacity[v])
            {
                maxWeightCapacity[v] = availableWeightCapacity;
                PushOrUpdateHeap(heap, v, availableWeightCapacity);
            }
        }
    }
}

void PrintAndSaveResults(const vector<int>& maxWeightCapacity, int countCities, const string& outputFilename)
{
    ofstream fout(outputFilename);
    for (int i = 2; i <= countCities; ++i)
    {
        cout << maxWeightCapacity[i] << endl;
        if (fout.is_open())
        {
            fout << maxWeightCapacity[i] << endl;
        }
    }
}


void Loop()
{
    string filename;
    cout << "=== Система расчета максимального груза (Задача 20) ===" << endl;

    while (true)
    {
        cout << "\nВведите имя файла с данными (например, INPUT.TXT или 'exit' для выхода): ";
        cin >> filename;

        if (filename == "exit" || filename == "quit" || filename == "q")
        {
            cout << "Завершение работы программы." << endl;
            break;
        }

        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "Ошибка: Не удалось открыть файл '" << filename << "'. Проверьте путь и повторите попытку." << endl;
            continue;
        }

        int countCities, countRoads;
        if (!(fin >> countCities >> countRoads))
        {
            cout << "Ошибка: Неверный формат данных в файле." << endl;
            continue;
        }

        vector<vector<Road>> roads(countCities + 1);
        ReadRoads(roads, countRoads, fin);

        vector<int> maxWeightCapacity;
        FindMaxWeightCapacity(countCities, roads, maxWeightCapacity);

        cout << "\n--- Результат анализа (файл: " << filename << ") ---" << endl;
        PrintAndSaveResults(maxWeightCapacity, countCities, DEFAULT_OUTPUT_FILE);
        cout << "Результат т/акже сохранен в файл " << DEFAULT_OUTPUT_FILE << endl;
        cout << "-----------------------------------------------" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.CP1251");
    Loop();
    return 0;
}