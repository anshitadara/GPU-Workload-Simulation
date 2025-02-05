# GPU-Workload-Simulation
Objective:
The goal of the project is to simulate the execution of tasks on multiple cores, mimicking a parallel processing environment, such as the one seen in GPUs. The simulation handles task assignment, execution, and completion tracking, while distributing the tasks across multiple cores (threads). This is useful for modeling workload distribution in a multi-core processing system.
Key Concepts Involved:
Task Generation:
The project begins by creating a list of tasks that need to be processed. Each task is represented by an object (or struct) that stores essential details such as its ID, execution time, and whether it has been completed.
Parallel Processing (Multithreading):
Tasks are processed in parallel by multiple "cores" (threads). Each core can process a task simultaneously, which speeds up the total execution time.
For each core, a separate thread is created that will pick tasks from the list and execute them.
Task Execution:
Each core processes the tasks sequentially. The output shows which core is processing which task and for how long.
The tasks have an execute() function that simulates the processing of the task (for example, by using sleep() to mimic the task duration).
Synchronization:
Once a core starts processing a task, it marks that task as completed, and other cores pick up the next available task.
Synchronization ensures that tasks are only processed once and that the order of processing is handled correctly across multiple threads.
Output:
The program outputs messages showing which core is processing which task and when each task is completed. It simulates a real-time multi-core system where tasks are assigned and processed concurrently.
Detailed Flow:
Task Creation:
The program generates a set of tasks using the generate_tasks() function. Each task has an ID and an execution time (in seconds).
Thread Creation:
Multiple threads (representing cores) are created using the std::thread library. Each thread processes tasks by calling the process_task() function.
Task Processing:
Each core processes tasks by checking if they are available for execution. The core picks the next available task, executes it, and marks it as complete.
Once a task is completed, the core proceeds to the next available task, ensuring that no task is processed more than once.
Completion Check:
The program checks when all tasks have been processed by all cores, ensuring that the simulation is complete.
Output Example:
If you had three tasks and four cores, the output might look like this:
arduino
CopyEdit
Core 0 is processing Task 1
Core 1 is processing Task 2
Core 2 is processing Task 3
Core 0 completed Task 1
Core 1 completed Task 2
Core 2 completed Task 3
All tasks processed!


This shows how tasks are distributed across cores and how they progress in parallel.
Project Structure and Code Breakdown:
Task Struct/Class:
Represents each task in the simulation.
Contains properties like id, execution time, and flags to track the task status (isCompleted).
process_task() Function:
Takes a core ID and processes tasks from the task list.
Handles task execution and completion for each core.
Multithreading:
Uses std::thread to simulate multiple cores processing tasks simultaneously.
Each thread picks tasks from the list and executes them.
Task Generation:
The generate_tasks() function creates a set of tasks with varying execution times and dependencies.
Synchronization:
Each core waits for its turn to process the next available task and waits for others to finish as necessary.
Output Example:
Here's what a sample output would look like after running the program:
arduino
CopyEdit
Core 0 is processing Task 1
Core 1 is processing Task 2
Core 2 is processing Task 3
Core 0 completed Task 1
Core 1 completed Task 2
Core 2 completed Task 3
All tasks processed!

How to Improve or Extend the Project:
Task Dependency:
Implement task dependencies where one task can only be processed after another task is completed.
Dynamic Task Assignment:
Instead of statically assigning tasks to cores, dynamically assign tasks to idle cores as they become available.
Error Handling:
Add error handling to deal with edge cases, such as when no tasks are available for processing.
Performance Monitoring:
Measure the total time taken to complete the tasks and compare the performance of the multi-core setup.
Why This Project is Useful:
This project demonstrates how parallel processing works in a multi-core system, which is essential for applications like GPUs, large-scale simulations, and real-time data processing. It also introduces the concept of using threads for concurrent execution, which is crucial in modern computing.












CODE:
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

class Task {
public:
    int id;
    int duration;
    bool isCompleted;

    Task(int id, int duration) : id(id), duration(duration), isCompleted(false) {}

    void execute() {
        std::this_thread::sleep_for(std::chrono::seconds(duration));  // Simulate task execution
        isCompleted = true;
    }

    bool canExecute() {
        return !isCompleted;
    }
};

std::vector<Task> generate_tasks() {
    return {Task(1, 2), Task(2, 3), Task(3, 1)};  // Create some sample tasks
}

std::queue<Task> taskQueue;
std::mutex taskMutex;

void process_task(int coreId) {
    while (true) {
        Task task(-1, -1);  // Initialize with invalid values

        {
            std::lock_guard<std::mutex> lock(taskMutex);  // Lock the queue to safely access tasks

            // If there are tasks left, pick one
            if (!taskQueue.empty()) {
                task = taskQueue.front();
                taskQueue.pop();  // Remove it from the queue
            }
        }

        if (task.id == -1) break;  // If no valid task was found, exit the loop

        std::cout << "Core " << coreId << " is processing Task " << task.id << std::endl;
        task.execute();
        std::cout << "Core " << coreId << " completed Task " << task.id << std::endl;
    }
}

int main() {
    std::vector<Task> tasks = generate_tasks();

    // Add tasks to the task queue
    for (auto& task : tasks) {
        taskQueue.push(task);
    }

    const int NUM_CORES = 4;

    std::vector<std::thread> cores;
    for (int i = 0; i < NUM_CORES; ++i) {
        cores.push_back(std::thread(process_task, i));  // Start a thread for each core
    }

    // Wait for all threads to finish
    for (auto& core : cores) {
        core.join();
    }

    std::cout << "All tasks processed!" << std::endl;
    return 0;
}



