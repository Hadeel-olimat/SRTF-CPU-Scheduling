#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Process {
    string id;
    int fileOrder;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int startTime = -1;
};

struct CompareProcess {
    bool operator()(Process const& p1, Process const& p2) {
        if (p1.remainingTime != p2.remainingTime)
            return p1.remainingTime > p2.remainingTime;

        if(p1.arrivalTime != p2.arrivalTime)
            return p1.arrivalTime < p2.arrivalTime;
        
        return p1.fileOrder > p2.fileOrder;
    }
};

struct CompareArrival {
    bool operator()(const Process& a, const Process& b) const {
        return a.arrivalTime < b.arrivalTime;
    }
};

int main() {
    ifstream inFile("in.txt");
    ofstream outFile("out.txt");

    vector <string> compList = {};

    int n;
    inFile >> n;
    vector<Process> processes;

    for (int i = 0; i < n; ++i) {
        Process temp;
        inFile >> temp.id >> temp.arrivalTime >> temp.burstTime;
        temp.remainingTime = temp.burstTime;
        temp.fileOrder = i;
        processes.push_back(temp);
    }

    sort(processes.begin(), processes.end(), CompareArrival());

    
    priority_queue<Process, vector<Process>, CompareProcess> pq;
    int currentTime = 0;
    int completed = 0;
    int i = 0;

    while (completed != n) {
        while (i < n && processes[i].arrivalTime <= currentTime) {
            pq.push(processes[i]);
            i++;
          
        }

        if (!pq.empty()) {
            Process current = pq.top();
            pq.pop();

            if (current.startTime == -1) {
                current.startTime = currentTime;
            }

            current.remainingTime--;
            currentTime++;

            if (current.remainingTime == 0) {
                current.completionTime = currentTime;
                completed++;

                int turnaroundTime = current.completionTime - current.arrivalTime;
                int waitingTime = turnaroundTime - current.burstTime;

                outFile << current.id << ": (end=" << current.completionTime
                        << ", turnaround=" << turnaroundTime
                        << ", delay=" << waitingTime
                        << ", response=" << current.startTime - current.arrivalTime << ")"
                        << endl;
                compList.push_back(current.id);
            } else {
                pq.push(current);
            }
        } else {
            currentTime++;
        }
    }

    for( auto i = compList.begin(); i != compList.end(); i++){
        outFile << *i ;
    }

    inFile.close();
    outFile.close();

    return 0;
}
