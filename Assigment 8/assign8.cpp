#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

using namespace std;

float seek_time = 0.0;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getBoundedInt(string prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value < maxVal)
            return value;
        else {
            cout << "Invalid input. Please enter a value between " << minVal << " and " << maxVal - 1 << ".\n";
            clearInput();
        }
    }
}

vector<int> getRequestQueue(int disk_size) {
    vector<int> queue;
    int n = getBoundedInt("Enter number of disk requests: ", 1, disk_size);
    cout << "Enter the disk requests (0 to " << disk_size - 1 << "):\n";
    for (int i = 0; i < n; ++i) {
        queue.push_back(getBoundedInt("  Request " + to_string(i + 1) + ": ", 0, disk_size));
    }
    return queue;
}

int fifo(const vector<int>& requests, int head, bool print = true) {
    int total = 0, current = head;
    for (int req : requests) {
        total += abs(current - req);
        current = req;
    }
    if (print) {
        cout << "\n--- FIFO (FCFS) ---\n";
        cout << "Total head movement: " << total << " cylinders\n";
        cout << "Total seek time: " << total * seek_time << " ms\n";
    }
    return total;
}

int sstf(vector<int> requests, int head, bool print = true) {
    int total = 0, current = head;
    while (!requests.empty()) {
        auto nearest = min_element(requests.begin(), requests.end(),
            [current](int a, int b) { return abs(current - a) < abs(current - b); });
        total += abs(current - *nearest);
        current = *nearest;
        requests.erase(nearest);
    }
    if (print) {
        cout << "\n--- SSTF ---\n";
        cout << "Total head movement: " << total << " cylinders\n";
        cout << "Total seek time: " << total * seek_time << " ms\n";
    }
    return total;
}

int scan(vector<int> requests, int head, int disk_size, string direction, bool print = true) {
    int total = 0, current = head;
    vector<int> left, right;
    for (int r : requests) {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    if (direction == "left") {
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs(current - left[i]);
            current = left[i];
        }
        if (!left.empty()) {
            total += abs(current - 0);
            current = 0;
        }
        for (int r : right) {
            total += abs(current - r);
            current = r;
        }
    } else {
        for (int r : right) {
            total += abs(current - r);
            current = r;
        }
        if (!right.empty()) {
            total += abs(current - (disk_size - 1));
            current = disk_size - 1;
        }
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs(current - left[i]);
            current = left[i];
        }
    }

    if (print) {
        cout << "\n--- SCAN (" << direction << ") ---\n";
        cout << "Total head movement: " << total << " cylinders\n";
        cout << "Total seek time: " << total * seek_time << " ms\n";
    }
    return total;
}

int cscan(vector<int> requests, int head, int disk_size, string direction = "right", bool print = true) {
    int total = 0;
    int current = head;

    vector<int> left, right;
    for (int r : requests) {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    if (direction == "right") {
        for (int r : right) {
            total += abs(current - r);
            current = r;
        }
        if (!right.empty()) {
            total += abs(current - (disk_size - 1));
            total += (disk_size - 1);  // jump to 0
            current = 0;
        }
        for (int r : left) {
            total += abs(current - r);
            current = r;
        }
    } else if (direction == "left") {
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs(current - left[i]);
            current = left[i];
        }
        if (!left.empty()) {
            total += abs(current - 0);
            total += (disk_size - 1); // jump from 0 to end
            current = disk_size - 1;
        }
        for (int i = right.size() - 1; i >= 0; --i) {
            total += abs(current - right[i]);
            current = right[i];
        }
    }

    if (print) {
        cout << "\n--- C-SCAN (" << direction << ") ---\n";
        cout << "Total head movement: " << total << " cylinders\n";
        cout << "Total seek time: " << total * seek_time << " ms\n";
    }
    return total;
}

void analyzeAll(const vector<int>& requests, int head, int disk_size) {
    cout << "\n--- Analysis ---\n";

    map<string, int> results;
    results["FIFO"] = fifo(requests, head, false);
    results["SSTF"] = sstf(requests, head, false);
    results["SCAN (left)"] = scan(requests, head, disk_size, "left", false);
    results["SCAN (right)"] = scan(requests, head, disk_size, "right", false);
    results["C-SCAN (left)"] = cscan(requests, head, disk_size, "left", false);
    results["C-SCAN (right)"] = cscan(requests, head, disk_size, "right", false);

    for (auto& [name, value] : results) {
        cout << name << ": " << value << " cylinders, " << value * seek_time << " ms\n";
    }

    auto best = min_element(results.begin(), results.end(),
                            [](const auto& a, const auto& b) { return a.second < b.second; });

    cout << "\n=> Best algorithm for this input: " << best->first
         << " (Total movement: " << best->second << " cylinders, " << best->second * seek_time << " ms)\n";
}

int main() {
    cout << "==== Disk Scheduling Simulator ====\n";

    int disk_size = getBoundedInt("Enter disk size (e.g., 200 for 0-199): ", 1, 10000);
    int head = getBoundedInt("Enter initial head position: ", 0, disk_size);
    seek_time = getBoundedInt("Enter seek time per cylinder (in ms, e.g., 1): ", 1, 10000);
    vector<int> requests = getRequestQueue(disk_size);

    while (true) {
        cout << "\n==== Menu ====\n";
        cout << "1. FIFO (FCFS)\n";
        cout << "2. SSTF\n";
        cout << "3. SCAN\n";
        cout << "4. C-SCAN\n";
        cout << "5. Analysis (Compare All)\n";
        cout << "6. Exit\n";

        int choice = getBoundedInt("Choose an option: ", 1, 7);

        switch (choice) {
            case 1:
                fifo(requests, head);
                break;
            case 2:
                sstf(requests, head);
                break;
            case 3: {
                string direction;
                while (true) {
                    cout << "Enter direction (left/right): ";
                    cin >> direction;
                    if (direction == "left" || direction == "right")
                        break;
                    else
                        cout << "Invalid direction. Try again.\n";
                }
                scan(requests, head, disk_size, direction);
                break;
            }
            case 4: {
                string direction;
                while (true) {
                    cout << "Enter direction (left/right): ";
                    cin >> direction;
                    if (direction == "left" || direction == "right")
                        break;
                    else
                        cout << "Invalid direction. Try again.\n";
                }
                cscan(requests, head, disk_size, direction);
                break;
            }
            case 5:
                analyzeAll(requests, head, disk_size);
                break;
            case 6:
                cout << "Exiting. Goodbye!\n";
                return 0;
        }
    }

    return 0;
}
