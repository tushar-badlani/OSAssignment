#include <bits/stdc++.h>

using namespace std;

vector<int> fcfs(vector<int> requests, int head, int seek)
{
    int current = head;
    vector<int> seq;
    int sum = 0;

    seq.push_back(current);

    for (int i = 0; i < requests.size(); i++)
    {
        seq.push_back(requests[i]);
        sum += abs(requests[i] - current);
        current = requests[i];
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

vector<int> sjf(vector<int> requests, int head, int seek)
{
    int current = head;
    vector<int> seq;
    int sum = 0;
    vector<bool> visited(requests.size(), false);

    seq.push_back(current);

    for (int i = 0; i < requests.size(); i++)
    {
        int min = INT_MAX;
        int index = -1;
        for (int j = 0; j < requests.size(); j++)
        {
            if (abs(requests[j] - current) < min && !visited[j])
            {
                min = abs(requests[j] - current);
                current = requests[j];
                index = j;
            }
        }
        sum += min;
        visited[index] = true;
        seq.push_back(current);
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

vector<int> scan_left(vector<int> requests, int head, int seek)
{
    vector<int> seq;
    sort(requests.begin(), requests.end());
    int current = head;
    int sum = 0;

    for (int i = requests.size() - 1; i >= 0; i--)
    {
        if (requests[i] < head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    sum += abs(0 - current);
    current = 0;
    seq.push_back(0);

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i] > head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

vector<int> scan_right(vector<int> requests, int head, int seek, int size)
{
    vector<int> seq;
    sort(requests.begin(), requests.end());
    int current = head;
    int sum = 0;

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i] > head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    sum += abs((size - 1) - current);
    current = size - 1;
    seq.push_back(size - 1);

    for (int i = requests.size() - 1; i >= 0; i--)
    {
        if (requests[i] < head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

vector<int> cscan_right(vector<int> requests, int head, int seek, int size)
{
    vector<int> seq;
    sort(requests.begin(), requests.end());
    int current = head;
    int sum = 0;

    for (int i = requests.size() - 1; i >= 0; i--)
    {
        if (requests[i] < head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    sum += abs(0 - current);
    current = 0;
    seq.push_back(current);

    sum += abs((size - 1) - current);
    current = size - 1;
    seq.push_back(size - 1);

    for (int i = requests.size() - 1; i >= 0; i--)
    {
        if (requests[i] > head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

vector<int> cscan_left(vector<int> requests, int head, int seek, int size)
{
    vector<int> seq;
    sort(requests.begin(), requests.end());
    int current = head;
    int sum = 0;

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i] > head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    sum += abs((size - 1) - current);
    current = size - 1;
    seq.push_back(size - 1);

    sum += abs(0 - current);
    current = 0;
    seq.push_back(current);

    for (int i = 0; i < requests.size(); i++)
    {
        if (requests[i] < head)
        {
            seq.push_back(requests[i]);
            sum += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements are: " << sum << endl;
    cout << "Total seek time is " << sum * seek << endl;
    return seq;
}

int main()
{
    int head = 50;
    int seek = 5;
    vector<int> requests = {20, 44, 89, 12, 77, 55, 7, 63, 33, 9};

    cout << "FCFS Order: ";
    vector<int> fcfs_order = fcfs(requests, head, seek);
    for (int i : fcfs_order)
        cout << i << " ";
    cout << endl;

    cout << "SJF Order: ";
    vector<int> sjf_order = sjf(requests, head, seek);
    for (int i : sjf_order)
        cout << i << " ";
    cout << endl;

    cout << "SCAN Left Order: ";
    vector<int> scan_left_order = scan_left(requests, head, seek);
    for (int i : scan_left_order)
        cout << i << " ";
    cout << endl;

    cout << "SCAN Right Order: ";
    vector<int> scan_right_order = scan_right(requests, head, seek, 100);
    for (int i : scan_right_order)
        cout << i << " ";
    cout << endl;

    cout << "CSCAN Left Order: ";
    vector<int> cscan_left_order = cscan_left(requests, head, seek, 100);
    for (int i : cscan_left_order)
        cout << i << " ";
    cout << endl;

    cout << "CSCAN Right Order: ";
    vector<int> cscan_right_order = cscan_right(requests, head, seek, 100);
    for (int i : cscan_right_order)
        cout << i << " ";
    cout << endl;
}