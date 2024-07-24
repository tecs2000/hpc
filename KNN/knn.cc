#include <bits/stdc++.h>
#include "knn.h"

using namespace std;

void print_project_info(char *exec_name)
{
    cout << exec_name << " Version " << Knn_VERSION_MAJOR
         << "." << Knn_VERSION_MINOR << endl;
}

int main(int argc, char *argv[])
{
    print_project_info(argv[0]);
    return 0;
}