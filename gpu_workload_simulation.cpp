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
