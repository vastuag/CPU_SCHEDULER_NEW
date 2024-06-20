#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <numeric>
#include <bits/stdc++.h>

using namespace std;

struct Process
{
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;
};

bool compareArrival(const Process &p1, const Process &p2)
{
    return (p1.arrival == p2.arrival) ? p1.id < p2.id : p1.arrival < p2.arrival;
}

bool compareBurst(const Process &p1, const Process &p2)
{
    return (p1.burst == p2.burst) ? p1.id < p2.id : p1.burst < p2.burst;
}

bool comparePriority(const Process &p1, const Process &p2)
{
    return (p1.arrival == p2.arrival) ? p1.priority < p2.priority : p1.arrival < p2.arrival;
}

void displayDetails(const vector<Process> &processes, const vector<int> &start, const vector<int> &completion, const vector<int> &waiting, const vector<int> &turnaround)
{
    cout << "\t+---------+--------------+------------+------------+-----------------+--------------+-----------------+\n";
    cout << "\t| Process | Arrival Time | Burst Time | Start Time | Completion Time | Waiting Time | Turnaround Time |\n";
    cout << "\t+---------+--------------+------------+------------+-----------------+--------------+-----------------+\n";
    for (size_t i = 0; i < processes.size(); ++i)
    {
        cout << "\t| " << setw(7) << processes[i].id
             << " | " << setw(12) << processes[i].arrival
             << " | " << setw(10) << processes[i].burst
             << " | " << setw(10) << start[i]
             << " | " << setw(15) << completion[i]
             << " | " << setw(12) << waiting[i]
             << " | " << setw(15) << turnaround[i] << " |\n";
    }
    cout << "\t+---------+--------------+------------+------------+-----------------+--------------+-----------------+\n";
}

void calculateAverage(const vector<int> &waiting, const vector<int> &turnaround)
{
    double avg_waiting = accumulate(waiting.begin(), waiting.end(), 0.0) / waiting.size();
    cout << "Average waiting time: " << avg_waiting << "\n";
    double avg_turnaround = accumulate(turnaround.begin(), turnaround.end(), 0.0) / turnaround.size();
    cout << "Average turnaround time: " << avg_turnaround << "\n";
}

void fcfs(const vector<int> &arrival, const vector<int> &burst)
{
    cout << "\nFirst Come First Serve (FCFS) Scheduling\n";
    cout << "---------------------------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    int current_time = 0;

    for (int i = 0; i < n; ++i)
    {
        if (current_time < processes[i].arrival)
        {
            cout << "CPU idle from " << current_time << " to " << processes[i].arrival << "\n";
            current_time = processes[i].arrival;
        }
        start[i] = current_time;
        waiting[i] = start[i] - processes[i].arrival;
        cout << "Process " << processes[i].id << " runs from " << current_time << " to " << current_time + processes[i].burst << "\n";
        current_time += processes[i].burst;
        completion[i] = current_time;
        turnaround[i] = completion[i] - processes[i].arrival;
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}

void sjf(const vector<int> &arrival, const vector<int> &burst)
{
    cout << "\nShortest Job First (SJF) Scheduling\n";
    cout << "-----------------------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    vector<bool> done(n, false);
    int current_time = 0, completed = 0;

    while (completed < n)
    {
        int shortest_index = -1;
        for (int i = 0; i < n; ++i)
        {
            if (!done[i] && processes[i].arrival <= current_time)
            {
                if (shortest_index == -1 || processes[i].burst < processes[shortest_index].burst)
                {
                    shortest_index = i;
                }
            }
        }

        if (shortest_index == -1)
        {
            current_time++;
            continue;
        }

        if (current_time < processes[shortest_index].arrival)
        {
            cout << "CPU idle from " << current_time << " to " << processes[shortest_index].arrival << "\n";
            current_time = processes[shortest_index].arrival;
        }

        start[shortest_index] = current_time;
        waiting[shortest_index] = start[shortest_index] - processes[shortest_index].arrival;
        cout << "Process " << processes[shortest_index].id << " runs from " << current_time << " to " << current_time + processes[shortest_index].burst << "\n";
        current_time += processes[shortest_index].burst;
        completion[shortest_index] = current_time;
        turnaround[shortest_index] = completion[shortest_index] - processes[shortest_index].arrival;
        done[shortest_index] = true;
        completed++;
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}

void rr(const vector<int> &arrival, const vector<int> &burst)
{
    cout << "\nRound Robin (RR) Scheduling\n";
    cout << "---------------------------\n\n";

    int time_quantum;
    cout << "Enter time quantum: ";
    cin >> time_quantum;

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
        processes[i].remaining = burst[i]; // initialize remaining time
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n, -1), completion(n), turnaround(n);
    queue<int> ready_queue;
    int current_time = 0, completed = 0, idx = 0;

    while (completed != n)
    {
        while (idx < n && processes[idx].arrival <= current_time)
        {
            ready_queue.push(idx);
            ++idx;
        }
        if (ready_queue.empty())
        {
            current_time = processes[idx].arrival;
            continue;
        }
        int proc = ready_queue.front();
        ready_queue.pop();
        if (start[proc] == -1)
        {
            start[proc] = current_time;
        }
        int exec_time = min(time_quantum, processes[proc].remaining);
        processes[proc].remaining -= exec_time;
        current_time += exec_time;

        if (processes[proc].remaining == 0)
        {
            completed++;
            completion[proc] = current_time;
            turnaround[proc] = completion[proc] - processes[proc].arrival;
            waiting[proc] = turnaround[proc] - processes[proc].burst;
        }

        while (idx < n && processes[idx].arrival <= current_time)
        {
            ready_queue.push(idx);
            ++idx;
        }

        if (processes[proc].remaining > 0)
        {
            ready_queue.push(proc);
        }
        cout << "Process " << processes[proc].id << " runs from " << current_time - exec_time << " to " << current_time << "\n";
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}

void priorityScheduling(const vector<int> &arrival, const vector<int> &burst)
{
    cout << "\nPriority Scheduling\n";
    cout << "-------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
        cout << "Enter priority for process " << i + 1 << ": ";
        cin >> processes[i].priority;
    }
    sort(processes.begin(), processes.end(), comparePriority);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    vector<bool> done(n, false);
    int current_time = 0, completed = 0;

    while (completed < n)
    {
        int highest_priority_index = -1;
        for (int i = 0; i < n; ++i)
        {
            if (!done[i] && processes[i].arrival <= current_time)
            {
                if (highest_priority_index == -1 || processes[i].priority < processes[highest_priority_index].priority)
                {
                    highest_priority_index = i;
                }
            }
        }

        if (highest_priority_index == -1)
        {
            current_time++;
            continue;
        }

        if (current_time < processes[highest_priority_index].arrival)
        {
            cout << "CPU idle from " << current_time << " to " << processes[highest_priority_index].arrival << "\n";
            current_time = processes[highest_priority_index].arrival;
        }

        start[highest_priority_index] = current_time;
        waiting[highest_priority_index] = start[highest_priority_index] - processes[highest_priority_index].arrival;
        cout << "Process " << processes[highest_priority_index].id << " runs from " << current_time << " to " << current_time + processes[highest_priority_index].burst << "\n";
        current_time += processes[highest_priority_index].burst;
        completion[highest_priority_index] = current_time;
        turnaround[highest_priority_index] = completion[highest_priority_index] - processes[highest_priority_index].arrival;
        done[highest_priority_index] = true;
        completed++;
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}

void srtf(const vector<int> &arrival, const vector<int> &burst)
{
    cout << "\nShortest Remaining Time First (SRTF) Scheduling\n";
    cout << "-----------------------------------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n), temp_burst(n), completion(n), turnaround(n);
    for (int i = 0; i < n; ++i)
    {
        temp_burst[i] = processes[i].burst;
    }

    int current_time = 0, completed = 0;

    while (completed != n)
    {
        int shortest = -1, shortest_time = INT_MAX;
        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrival <= current_time && temp_burst[i] > 0 && temp_burst[i] < shortest_time)
            {
                shortest = i;
                shortest_time = temp_burst[i];
            }
        }
        if (shortest == -1)
        {
            current_time++;
            continue;
        }
        if (temp_burst[shortest] == processes[shortest].burst)
        {
            start[shortest] = max(current_time, processes[shortest].arrival);
        }
        temp_burst[shortest]--;
        current_time++;

        if (temp_burst[shortest] == 0)
        {
            completed++;
            waiting[shortest] = current_time - processes[shortest].arrival - processes[shortest].burst;
            completion[shortest] = current_time;
            turnaround[shortest] = completion[shortest] - processes[shortest].arrival;
        }
        cout << "Process " << processes[shortest].id << " runs at time " << current_time << "\n";
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}

int main()
{
    int choice;
    int process_count;

    cout << "Please enter the number of processes: ";
    cin >> process_count;
    vector<int> arrival(process_count);
    vector<int> burst(process_count);

    cout << "Enter (Arrival_time, Burst_time) of the processes:\n";
    for (int i = 0; i < process_count; ++i)
    {
        cout << "Process " << i + 1 << ": ";
        cin >> arrival[i];
        cin >> burst[i];
    }

    cout << "\nCPU Scheduling Algorithms\n";
    cout << "1. First Come First Serve (FCFS)\n";
    cout << "2. Shortest Job First (SJF)\n";
    cout << "3. Round Robin (RR)\n";
    cout << "4. Priority Scheduling\n";
    cout << "5. Shortest Remaining Time First (SRTF)\n";
    cout << "6. Terminate the program\n";
    cin >> choice;

    switch (choice)
    {
    case 1:
        fcfs(arrival, burst);
        break;
    case 2:
        sjf(arrival, burst);
        break;
    case 3:
        rr(arrival, burst);
        break;
    case 4:
        priorityScheduling(arrival, burst);
        break;
    case 5:
        srtf(arrival, burst);
        break;
    case 6:
        cout << "Exiting...\n";
        break;
    default:
        cout << "Invalid choice. Please try again.\n";
    }

    return 0;
}