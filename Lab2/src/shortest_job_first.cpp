#include "shortest_job_first.hpp"

#include <algorithm>

namespace cse4733
{

    // Comparator function for SJF scheduling
    auto shortest_job_first::operator()(const std::shared_ptr<cse4733::IProcess> &a,
                                        const std::shared_ptr<cse4733::IProcess> &b) -> bool
    {
        return a->get_burst_time() < b->get_burst_time();
    }

    void shortest_job_first::run(std::vector<std::shared_ptr<cse4733::IProcess>> &processes)
    {
        int completion_time = 0;
        
        // TODO:
        // Sort processes by using the shortest job first comparator
        // For all processes
        //   Get the burst time
        //   Set the completion time to the current time plus the burst time
        //   Set the turnaround time to the completion time minus the arrival time
        //   Set the waiting time to the completion time minus the burst time
        //   Add burst time to completion time
    }

    void shortest_job_first::print_results(std::vector<std::shared_ptr<cse4733::IProcess>> &processes)
    {
        int total_completion_time = 0;
        int total_waiting = 0;
        int total_turnaround = 0;
        int n = processes.size();

        std::cout << "SJF Scheduling:\n";
        std::cout << "Process ID\tCompletion Time\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n";
        for (const auto &item : processes)
        {
            std::cout << "  " << item->to_string() << std::endl;
            total_turnaround += item->get_turnaround_time();
            total_waiting += item->get_waiting_time();
            total_completion_time += item->get_burst_time();
        }

        double average_waiting_time = static_cast<double>(total_waiting) / n;
        double average_turnaround_time = static_cast<double>(total_turnaround) / n;

        std::cout << "  Average Waiting Time: " << average_waiting_time << std::endl;
        std::cout << "  Average Turnaround Time: " << average_turnaround_time << std::endl
                  << std::endl;
    }

}
