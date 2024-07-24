#include <bits/stdc++.h>
#include "knn.h"

using namespace std;

class Point
{
public:
    vector<float> features;
    char category;
};

vector<Point> parse_file(char *path);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <path-to-file> <k>" << std::endl;
        return 1;
    }

    vector<Point> dataset = parse_file(argv[1]);

    return 0;
}

vector<Point> parse_file(char *path)
{
    vector<Point> dataset;

    freopen(path, "r", stdin);

    string row;
    while (getline(cin, row))
    {
        Point entry;
        string digit = "";
        for (int i = 0; i < row.size(); i++)
        {
            if (i == row.size() - 1) {
                entry.category = digit[0];
            }
            else if (row[i] != ',') {
                digit += row[i];
            }
            else
            {
                entry.features.push_back(stof(digit));
                digit = "";
            }
        }

        dataset.push_back(entry);
    }

    return dataset;
}