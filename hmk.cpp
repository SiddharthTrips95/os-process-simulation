#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int priority;   // Priority
    int remaining;  // Remaining Time
    int completion; // Completion Time
};

// Comparator to sort by arrival time, then by priority
bool compareByArrivalAndPriority(const Process &a, const Process &b) {
    if (a.arrival == b.arrival) {
        return a.priority < b.priority;  // If arrival is the same, check priority
    }
    return a.arrival < b.arrival;
}

// Function to simulate the FCFS with priority and time quantum scheduling algorithm
void simulateScheduling(vector<Process> &processes, int timeQuantum) {
    int currentTime = 0; // Current time in the simulation
    vector<Process> readyQueue; // Ready queue for processes
    int n = processes.size();  // Number of processes
    vector<bool> finished(n, false);  // To track finished processes
    vector<int> waitingTime(n, 0), turnaroundTime(n, 0); // To store waiting and turnaround times

    // Sort processes by arrival time and priority
    sort(processes.begin(), processes.end(), compareByArrivalAndPriority);

    int idx = 0; // To track next process to add to the ready queue

    // Simulate until all processes are complete
    while (idx < n || !readyQueue.empty()) {
        // Add all arrived processes to the ready queue based on the current time
        while (idx < n && processes[idx].arrival <= currentTime) {
            readyQueue.push_back(processes[idx]);
            idx++;
        }

        // Sort the ready queue based on priority
        sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
            return a.priority < b.priority;  // Higher priority runs first
        });

        if (!readyQueue.empty()) {
            Process currentProcess = readyQueue.front();
            readyQueue.erase(readyQueue.begin());  // Remove the first process from the queue

            // Calculate the time to run (either the remaining time or time quantum)
            int runTime = min(timeQuantum, currentProcess.remaining);
            currentTime += runTime;
            currentProcess.remaining -= runTime;

            // If the process is finished
            if (currentProcess.remaining == 0) {
                currentProcess.completion = currentTime;
                int pidIndex = currentProcess.pid - 1;

                finished[pidIndex] = true;
                turnaroundTime[pidIndex] = currentProcess.completion - processes[pidIndex].arrival;
                waitingTime[pidIndex] = turnaroundTime[pidIndex] - processes[pidIndex].burst;

                // Update the process information in the original array
                processes[pidIndex].completion = currentProcess.completion;
            } else {
                // Process is not finished, put it back in the queue
                readyQueue.push_back(currentProcess);
            }
        } else {
            // If no process is in the queue, increment the current time
            currentTime++;
        }
    }

    // Display results
    cout << "PID\tArrival\tBurst\tPriority\tCompletion\tTurnaround\tWaiting\n";
    for (int i = 0; i < n; i++) {
        cout << processes[i].pid << "\t" << processes[i].arrival << "\t"
             << processes[i].burst << "\t" << processes[i].priority << "\t\t"
             << processes[i].completion << "\t\t" << turnaroundTime[i] << "\t\t" 
             << waitingTime[i] << "\n";
    }

    // Calculate and display average turnaround and waiting times
    double totalTurnaround = 0, totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        totalTurnaround += turnaroundTime[i];
        totalWaiting += waitingTime[i];
    }

    cout << "\nAverage Turnaround Time: " << totalTurnaround / n << "\n";
    cout << "Average Waiting Time: " << totalWaiting / n << "\n";
}

int main() {
    int n;  // Number of processes
    int timeQuantum;  // Time quantum

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter time quantum: ";
    cin >> timeQuantum;

    vector<Process> processes(n);  // Vector to store process details

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;  // Process ID starts from 1
        cout << "Enter arrival time, burst time and priority for process " << i + 1 << ": ";
        cin >> processes[i].arrival >> processes[i].burst >> processes[i].priority;
        processes[i].remaining = processes[i].burst;  // Initialize remaining time as burst time
    }

    simulateScheduling(processes, timeQuantum);

    return 0;
}

