# *CpuScheduler*

It is a system software component in an operating system that is responsible for managing process execution by CPU. Its function is to decide which process in the ready queue should be executed by CPU, allocation of that process to CPU acc. to specific algorithm. It also ensures that processes get appropriate CPU time for execution, especially in time shared systems.
CPU Scheduling is very important because, 
By scheduling, we can ensure the CPU has minimal idle time and being maximum utilized. 
It is used to increase the number of processes completed per unit time. Higher the throughput, the more the system can perform/handle the tasks.
Scheduling ensures quick responses to the user inputs, enhancing the user experience by reducing the response time.
It enables multiple processes to run concurrently, making multitasking possible.

**CPU scheduling decisions only being taken under the following 4 circumstances**
 - When a process switches from running state to waiting state. [Non Preemptive Scheduling]
 - When a process switches from running state to ready state. [Preemptive Scheduling] – for e.g., when an interrupt occurs.
 - When a process switches from waiting state to ready state. [Preemptive Scheduling] – for e.g., at completion of I/O.
 - When a process terminates.

**Scheduling Criteria**  
 Scheduling criteria is crucial for selecting or designing an appropriate scheduling algorithm that meets the specific needs of a computing environment.
	
 - Maximize CPU utilization to ensure the CPU is not idle.
 - Maximize throughput to handle more processes in less time.
 - Minimize turnaround time [i.e. time taken from the submission of a process to its completion] to ensure processes are completed quickly.
 - Minimize waiting time [i.e. total time a process spends waiting in the ready queue] to reduce delays in process execution.
 - Minimize response time [i.e. time from the submission of a request until the first response is produced] to improve the interactivity of the system, especially important for time-sharing systems.

## **Scheduling Algorithms** 
	
### **First Come First Served Scheduling**

- Simplest Scheduling Algorithm 
- In this, the CPU will be allocated to the process that requests the CPU first. The CPU picks the process at the front of the ready queue and executes it until completion or it moves to the waiting state.
- Non Preemptive type algorithm i.e., once a process starts executing, it runs to completion without being preempted.

**Implementation:**
			
```cpp

void fcfs(const vector<int> &arrival, const vector<int> &burst) {
    cout << "\nFirst Come First Serve (FCFS) Scheduling\n";
    cout << "---------------------------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    int current_time = 0;

    for (int i = 0; i < n; ++i) {
        if (current_time < processes[i].arrival) {
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
```

**Initialization-**  
 - Determine the number of processes.  
 - Create and initialize a vector of Process structures with IDs, arrival times, burst times, and priority (set to 0 here).
 - Sort the processes by arrival time.
   
**Execution-**  
 - Create vectors to store waiting, start, completion, and turnaround times for each process.
 - Initialize current_time to 0.
 - Loop through each process:
   - If the CPU is idle (current time is less than the process's arrival time), update the current time to the arrival time.
   - Record the start time and calculate the waiting time.
   - Print the execution interval of the process.
   - Update the current time after the process finishes execution.
   - Record the completion and turnaround times.  

**Output-**
- Display details of each process using displayDetails.
- Calculate and display average waiting and turnaround times using calculateAverage.


One of the advantage of using this algorithm is its simplicity i.e., it is easy to understand and implement.  

**Limitations:-**  

Convoy Effect:      
Short processes may have to wait for long processes to complete, leading to the "convoy effect," where shorter tasks are delayed excessively.  
The average waiting under the FCFS algorithm is quite long which makes it troublesome to use in a time-shared system.  

FCFS is ideal for environments that prioritize simplicity and predictability over performance optimization.  
	

### **Shortest Job First Scheduling**  

- It selects the process with the smallest amount of time remaining until completion. If a new process arrives with a shorter remaining time than the current process, the current process is interrupted and the new process is executed.  
- Preemptive/Non-Preemptive type algorithm. I had consider it to be Preemptive(SRTF) and Non-Preemptive(SJF) both i.e., processes can be interrupted and moved to the ready queue if a new process arrives with a shorter remaining time.  

**Implementation:**
		
SJF algorithm:
```cpp		
void sjf(const vector<int> &arrival, const vector<int> &burst) {
    cout << "\nShortest Job First (SJF) Scheduling\n";
    cout << "-----------------------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    vector<bool> done(n, false);
    int current_time = 0, completed = 0;

    while (completed < n) {
        int shortest_index = -1;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= current_time) {
                if (shortest_index == -1 || processes[i].burst < processes[shortest_index].burst) {
                    shortest_index = i;
                }
            }
        }

        if (shortest_index == -1) {
            current_time++;
            continue;
        }

        if (current_time < processes[shortest_index].arrival) {
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
```

**Initialization-**
- Process Setup: Convert arrival and burst times into Process objects and sort by arrival time.
- Data Structures: Initialize vectors for waiting times, start times, completion times, and turnaround times. Use a boolean vector to track completed processes.

**Simulation Loop-**
- Find Shortest Job: Within the loop, find the shortest job (process with the smallest burst time) that has arrived and isn't completed.
- Handle Idle Time: If no process is ready to run, increment current_time until a process arrives.

**Process Execution-**
- Idle Check: If the current time is less than the process's arrival time, update and print idle period.
- Update Times: Record start time, calculate waiting time, and update current time. Print execution interval, and record completion and turnaround times. Mark process as done.

**Output-**
- Display Results: Use displayDetails to print process details.
- Average Calculations: Calculate and display average waiting and turnaround times using calculateAverage.  


SRTF algorithm:
```cpp	
void srtf(const vector<int> &arrival, const vector<int> &burst) {
  cout << "\nShortest Remaining Time First (SRTF) Scheduling\n";
  cout << "-----------------------------------------------\n\n";

  int n = arrival.size();
  vector<Process> processes(n);
  for (int i = 0; i < n; ++i) {
      processes[i] = {i + 1, arrival[i], burst[i], 0};
  }
  sort(processes.begin(), processes.end(), compareArrival);

  vector<int> waiting(n), start(n), temp_burst(n), completion(n), turnaround(n);
  for (int i = 0; i < n; ++i) {
      temp_burst[i] = processes[i].burst;
  }

  int current_time = 0, completed = 0;

  while (completed != n) {
      int shortest = -1, shortest_time = INT_MAX;
      for (int i = 0; i < n; ++i) {
          if (processes[i].arrival <= current_time && temp_burst[i] > 0 && temp_burst[i] < shortest_time) {
              shortest = i;
              shortest_time = temp_burst[i];
          }
      }
      if (shortest == -1) {
          current_time++;
          continue;
      }
      if (temp_burst[shortest] == processes[shortest].burst) {
          start[shortest] = max(current_time, processes[shortest].arrival);
      }
      temp_burst[shortest]--;
      current_time++;

      if (temp_burst[shortest] == 0) {
          Completed++;
          waiting[shortest] = current_time - processes[shortest].arrival - processes[shortest].burst;
          completion[shortest] = current_time;
          turnaround[shortest] = completion[shortest] - processes[shortest].arrival;
      }
      cout << "Process " << processes[shortest].id << " runs at time " << current_time << "\n";
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}
```		

**Initialization-**
- Setup Processes: Create a vector of Process objects with arrival and burst times, and sort them by arrival time.
- Auxiliary Vectors: Initialize vectors for waiting times, start times, completion times, and turnaround times. Create a temporary burst time vector temp_burst to track remaining burst times.
  
**Simulation Loop-**
- Find Process with Shortest Remaining Time: In each iteration, identify the process with the smallest remaining burst time that has arrived.
- Handle Idle Time: If no process is ready, increment current_time until a process arrives.

**Process Execution-**
- Start Time Recording: If the process is starting for the first time, record its start time.
- Update Burst Time: Decrement the remaining burst time of the chosen process and increment current_time.
- Completion Check: If the process completes (remaining burst time becomes zero), update completion, waiting, and turnaround times. Increment the completed count.
- Execution Logging: Print a message indicating which process is running at the current time.
  
**Output-**
- Display Results: Call displayDetails to print process details.
- Calculate Averages: Use calculateAverage to compute and display average waiting and turnaround times.


**Advantages-**

- SJF/SRTF is optimal for minimizing the average waiting time among all scheduling algorithms.
- Prioritizes shorter processes, making it highly responsive to processes with shorter burst times.

**Disadvantages-**

- Longer processes may experience quite long waiting time if short processes keep arriving that lead to a situation of Starvation.
- Although the SJF algorithm is optimal, it can not be implemented at the level of short-term CPU scheduling.
- Needs Prediction: Requires accurate knowledge or prediction of the remaining burst time for each process, which is not feasible.

SJF/SRTF is best for environments needing optimal average waiting times and quick responses for short tasks but may involve complex implementation and potential starvation of longer tasks.


### **Priority Scheduling**

- It selects the process based on their priority. Each process is assigned a priority, and the CPU is allocated to the process with the highest priority. If two processes have the same priority, they are scheduled according to their arrival order.
- Preemptive/Non-Preemptive type algorithm. I had considered it to be Non-Preemptive i.e., if a new process arrives with a higher priority than the currently running process, the current process is preempted.

**Implementation-**
```cpp
void priorityScheduling(const vector<int> &arrival, const vector<int> &burst) {
    cout << "\nPriority Scheduling\n";
    cout << "-------------------\n\n";

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
        cout << "Enter priority for process " << i + 1 << ": ";
        cin >> processes[i].priority;
    }
    sort(processes.begin(), processes.end(), comparePriority);

    vector<int> waiting(n), start(n), completion(n), turnaround(n);
    vector<bool> done(n, false);
    int current_time = 0, completed = 0;

    while (completed < n) {
        int highest_priority_index = -1;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && processes[i].arrival <= current_time) {
              if (highest_priority_index == -1 || processes[i].priority < processes[highest_priority_index].priority) {
                    highest_priority_index = i;
                }
            }
        }

        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }

        if (current_time < processes[highest_priority_index].arrival) {
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
```


**Initialization-**
- Setup Processes: Create a vector of Process objects with arrival and burst times. Prompt the user to input the priority for each process.
- Sort by Priority: Sort processes by their priority using the comparePriority function.

**Simulation Loop-**
- Find Process with Highest Priority: In each iteration, identify the highest-priority process that has arrived and is not yet completed.
- Handle Idle Time: If no process is ready, increment current_time until a process arrives.
  
**Process Execution-**
- Start Time Recording: Record the start time of the selected process.
- Update Current Time: Update current_time by adding the burst time of the selected process.
- Completion Metrics: Calculate waiting, completion, and turnaround times for the process. Mark it as done and increment the completed count.
- Execution Logging: Print a message indicating which process is running and for how long.

**Output-**
- Display Results: Call displayDetails to print process details.
- Calculate Averages: Use calculateAverage to compute and display average waiting and turnaround times.
	


**Advantages-**

- Can assign priorities based on importance, urgency, or other criteria.
- Efficient for Critical Tasks: Ensures that high-priority tasks are executed quickly.

**Disadvantages-**

- Starvation: Low-priority processes may suffer indefinite postponement if higher-priority processes keep arriving.	
- Aging Required: To prevent starvation, aging techniques may need to be implemented, where the priority of a process increases the longer it waits.

Priority scheduling is best for systems needing to handle tasks with varying importance or urgency but requires careful management to avoid starvation of low-priority tasks.


### **Round Robin Scheduling**
	
- Simple preemptive CPU scheduling algorithm
- Each process is assigned a fixed time slice or time quantum. The CPU scheduler cyclically switches between processes, allocating each process a time quantum to execute. If a process completes its execution within the time quantum, it is removed from the queue. If not, it is moved to the end of the queue to await its next turn.

**Implementation:**
```cpp	
void rr(const vector<int> &arrival, const vector<int> &burst) {
    cout << "\nRound Robin (RR) Scheduling\n";
    cout << "---------------------------\n\n";

    int time_quantum;
    cout << "Enter time quantum: ";
    cin >> time_quantum;

    int n = arrival.size();
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i] = {i + 1, arrival[i], burst[i], 0};
        processes[i].remaining = burst[i];  // initialize remaining time
    }
    sort(processes.begin(), processes.end(), compareArrival);

    vector<int> waiting(n), start(n, -1), completion(n), turnaround(n);
    queue<int> ready_queue;
    int current_time = 0, completed = 0, idx = 0;

    while (completed != n) {
        while (idx < n && processes[idx].arrival <= current_time) {
            ready_queue.push(idx);
            ++idx;
        }
        if (ready_queue.empty()) {
            current_time = processes[idx].arrival;
            continue;
        }
        int proc = ready_queue.front();
        ready_queue.pop();
        if (start[proc] == -1) {
            start[proc] = current_time;
        }
        int exec_time = min(time_quantum, processes[proc].remaining);
        processes[proc].remaining -= exec_time;
        current_time += exec_time;

        if (processes[proc].remaining == 0) {
            completed++;
            completion[proc] = current_time;
            turnaround[proc] = completion[proc] - processes[proc].arrival;
            waiting[proc] = turnaround[proc] - processes[proc].burst;
        }

        while (idx < n && processes[idx].arrival <= current_time) {
            ready_queue.push(idx);
            ++idx;
        }

        if (processes[proc].remaining > 0) {
            ready_queue.push(proc);
        }
        cout << "Process " << processes[proc].id << " runs from " << current_time - exec_time << " to " << current_time << "\n";
    }

    displayDetails(processes, start, completion, waiting, turnaround);
    calculateAverage(waiting, turnaround);
}
```

**Initialization-**
- Time Quantum Input: Prompts the user to input the time quantum.
- Process Setup: Initializes a list of Process objects with IDs, arrival times, burst times, and remaining burst times.
- Sort by Arrival Time: Sorts processes based on their arrival times for easier handling.
  
**Variables-**
- Tracking Metrics: Initializes vectors for waiting times, start times, completion times, and turnaround times.
- Queue: Uses a queue to manage the ready processes.
- Current Time and Counters: Tracks the current time, completed processes count, and index for iterating through processes.
  
**Main Loop-**
- Add Arrived Processes: Adds processes that have arrived by the current time to the queue.
- CPU Idle Handling: If no process is ready, advances the current time to the next process arrival time.
- Process Execution:
  - Start Time Recording: Records the start time of a process if it’s the first execution.
  - Execute Process: Runs the process for the time quantum or until it finishes, whichever is shorter. Updates remaining burst time and current time.
  - Completion Check: If the process finishes, updates its completion, turnaround, and waiting times.
- Requeue and Output:
  - Requeue Process: If the process is not finished, it is re-added to the queue.
  - Print Execution Time: Logs the execution start and end times for the process.
**Results Output-**
- Display Details: Calls displayDetails to print all process details.
- Calculate Averages: Calls calculateAverage to compute and print average waiting and turnaround times.

**Advantages-**

- Fairness: Provides fair allocation of CPU time among all processes.
- Efficient for Time-Sharing Systems: Suitable for time-sharing systems where multiple users require quick responses.

**Disadvantages-**

- Higher Turnaround Time for Longer Processes: Longer processes may need to wait for multiple time slices before completing, leading to higher turnaround time.
- Poor Performance with Uneven Time Quantums: If the time quantum is too large, RR can resemble FCFS, and if it's too small, there's increased overhead due to frequent context switching.

Round Robin is best for time-sharing and interactive systems where fairness and responsiveness are essential, but requires careful selection of the time quantum to balance efficiency and performance.


## Conclusion:

In this project, I explored various CPU scheduling algorithms, including First-Come, First-Served (FCFS), Shortest Job First (SJF), Shortest Remaining Time First (SRTF), Priority Scheduling, and Round Robin (RR). Each algorithm offers unique advantages and challenges, making them suitable for different system requirements and workloads.

FCFS is straightforward but can lead to inefficiencies for shorter tasks. SJF/SRTF optimizes waiting time but can starve longer processes. Priority Scheduling allows flexibility in process handling but requires careful management to prevent starvation. Round Robin ensures fairness and responsiveness, ideal for interactive systems, but requires an appropriately chosen time quantum.

Understanding these algorithms and their trade-offs is crucial for designing effective CPU schedulers that balance performance, fairness, and complexity. This project provides a comprehensive overview of CPU scheduling, enabling more informed decisions in process management and system optimization.
