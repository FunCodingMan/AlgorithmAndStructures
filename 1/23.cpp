#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

struct Pupil
{
    string surname;
    string name;
    int marksSum;
};


void printTopPupils(const vector<Pupil>& pupils)
{
    if (pupils.empty()) return;

    int minScore = (pupils.size() >= 3) ? pupils[2].marksSum : pupils.back().marksSum;

    for (const Pupil& pupil: pupils)
    {
        if (pupil.marksSum < minScore) break;
        cout << pupil.surname << " " << pupil.name << "\n";
    }
}

void sortPupils(vector<Pupil>& pupils)
{
    int n = pupils.size();

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (pupils[j].marksSum < pupils[j + 1].marksSum)
            {
                swap(pupils[j], pupils[j+1]);
            }
        }
    }
}

void readPupils(vector<Pupil>& pupils, int count)
{
    pupils.resize(count);

    for (int i = 0; i < count; ++i)
    {
        int m1, m2, m3;
        cin >> pupils[i].surname >> pupils[i].name >> m1 >> m2 >> m3;
        pupils[i].marksSum = m1 + m2 + m3;
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
