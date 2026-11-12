#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <locale.h>
#include <string>


const int INF = 1e9;
const int UNREACHABLE = -1;
const int START_CITY = 1;
const string DEFAULT_OUTPUT_FILE = "OUTPUT.TXT";

using namespace std;

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

        vector<int> maxCargo;
        // FindMaxCargo(countCities, roads, maxCargo);

        cout << "\n--- Результат анализа (файл: " << filename << ") ---" << endl;
        // PrintAndSaveResults(maxCargo, countCities, DEFAULT_OUTPUT_FILE);
        // cout << "Результат также сохранен в файл " << DEFAULT_OUTPUT_FILE << endl;
        cout << "-----------------------------------------------" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.CP1251");
    Loop();
    return 0;
}