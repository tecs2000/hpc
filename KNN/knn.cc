#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include <cmath>
#include <chrono>
#include "knn.h"

using namespace std;

class Entry
{
public:
    vector<float> features;
    string category;
};

int k;
int preprocess;
string trainset_path;
string testset_path;

vector<Entry> trainset;
vector<Entry> testset;

vector<Entry> process_csv(bool isTestset);
vector<Entry> process_testset();
string classify_entry(const Entry &point);
void print_dataset(const vector<Entry> &dataset);
void process_cmdline(int argc, char *argv[]);
float compute_euclidean_distance(const Entry &point, const Entry &entry);

int main(int argc, char *argv[])
{

    process_cmdline(argc, argv);   // ensure all global variables are filled
    trainset = process_csv(false); // process csv file and stores in dataset var
    testset = process_testset();

    auto start = chrono::high_resolution_clock::now();

    float error = 0;
    for (const auto &entry : testset)
    {
        string result = classify_entry(entry);
        string original = entry.category;
        if(result != original) ++error;
    }

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << (float)duration.count() / 1000 << " seconds" << endl;
    cout << "Error rate: " << (float)error / testset.size() << "%" << endl;

    return 0;
}

float compute_euclidean_distance(const Entry &point, const Entry &entry)
{
    return sqrt(inner_product(
        point.features.begin(), point.features.end(),
        entry.features.begin(), 0.0f,
        plus<float>(), [](float a, float b)
        { return pow(a - b, 2); }));
}

string classify_entry(const Entry &point)
{
    vector<pair<float, string>> distances;
    distances.reserve(trainset.size());

    for (const auto &entry : trainset)
    {
        float euc_dist = compute_euclidean_distance(point, entry);
        distances.push_back({euc_dist, entry.category});
    }

    sort(distances.begin(), distances.end());

    unordered_map<string, int> voting;
    pair<string, int> winner = {"", 0};

    for (int i = 0; i < k; i++)
    {
        const string &category = distances[i].second;
        int count = ++voting[category];

        if (count > winner.second)
            winner = {category, count};
    }

    return winner.first;
}

void process_cmdline(int argc, char *argv[])
{
    if (argc < 4 || argc > 5)
    {
        cout << "Usage: " << argv[0] << " <trset-path> <tstset-path>[optional] <preprocess>[1, 0] <k>" << std::endl;
        exit(1);
    }
    else
    {
        trainset_path = argv[1];
        if (argc == 5)
        {
            testset_path = argv[2];
            preprocess = stoi(argv[3]);
            k = stoi(argv[4]);
        }
        else
        {
            preprocess = stoi(argv[2]);
            k = stoi(argv[3]);
        }
    }

    // Additional checks to ensure k and preprocess are valid
    if (k <= 0)
    {
        cerr << "Invalid value for k. It must be greater than 0." << endl;
        exit(1);
    }

    if (preprocess != 0 && preprocess != 1)
    {
        cerr << "Invalid value for preprocess. It must be 0 or 1." << endl;
        exit(1);
    }
}

void print_dataset(const vector<Entry> &dataset)
{
    for (const auto &entry : dataset)
    {
        for (const auto &feature : entry.features)
        {
            cout << feature << " ";
        }
        cout << entry.category << endl;
    }
}

vector<Entry> process_testset()
{
    vector<Entry> dataset;

    if (testset_path.empty()) // creates a testset
    {
        random_shuffle(trainset.begin(), trainset.end());

        size_t end = trainset.size() / 4; // the test set will contain 1/4 of the total data

        dataset.assign(trainset.begin(), trainset.begin() + end);
        trainset.erase(trainset.begin(), trainset.begin() + end);
    }
    else
    {
        dataset = process_csv(true);
    }

    return dataset;
}

vector<Entry> process_csv(bool isTestset)
{
    vector<Entry> dataset;
    string path = isTestset ? testset_path : trainset_path;

    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Failed to open the file: " << path << endl;
        exit(1);
    }

    string row;
    if (preprocess)
        getline(file, row); // skips file first row

    while (getline(file, row))
    {
        Entry entry;
        stringstream ss(row);
        string value;

        if (preprocess)
            getline(ss, value, ','); // discards the entry id

        while (getline(ss, value, ','))
        {
            try
            {
                entry.features.push_back(stof(value));
            }
            catch (const invalid_argument &e)
            {
                entry.category = entry.features.back();
            }
        }

        dataset.push_back(entry);
    }

    file.close();
    return dataset;
}
