#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <boost/program_options.hpp>

// Thread local storage for the number
thread_local int prime = 2;

bool isPrime(int number)
{
    if (number < 2)
        return false;
    for (int i = 2; i * i <= number; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

void work(int id, std::mutex &cout_mutex)
{
    int number = 2;
    const auto time_limit = std::chrono::seconds(30);
    const auto start_time = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
    while (elapsed < time_limit)
    {
        if (isPrime(number))
        {
            prime = number;
        }
        ++number;
        std::this_thread::yield();
        elapsed = std::chrono::high_resolution_clock::now() - start_time;
    }
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        std::cout << "Thread " << id << " ended. Total run time: " 
                << total_time << " seconds. Largest prime calculated: " 
                << prime << std::endl;
    }

    // The thread will run for 30 seconds, checking for prime numbers.
    //
    // 1. Initialize number to 2.
    // 2. Set a time limit of 30 seconds using std::chrono::seconds.
    // 3. Record the start time using std::chrono::high_resolution_clock.
    // 4. Initialize elapsed time to zero.
    // 5. While elapsed time is less than the time limit:
    //    a. If number is prime, update the thread-local prime variable.
    //    b. Increment number.
    //    c. Yield the current thread to allow other threads to run.
    //    d. Update elapsed time by subtracting start time from current time.
    // 6. After exiting the loop, print the thread id, total run time, and largest prime number
}

int main(int argc, char *argv[])
{
    try
    {

        boost::program_options::options_description desc("Allowed options");
        desc.add_options()("help", "produce help message")("threads", boost::program_options::value<int>()->required(), "set the number of threads");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        int num_threads = vm["threads"].as<int>();
        std::vector<std::thread> threads;
        std::mutex cout_mutex;
        for (int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(work, i, std::ref(cout_mutex));
        }
        for (auto &t : threads)
        {
            t.join();
        }
        /*
            1. Parse command line arguments for number of threads.
            2. If "help" is requested, print help and exit.
            3. If "threads" is specified, set num_threads to its value.
            4. Otherwise, print help and exit.

            5. Create a vector to hold thread objects.
            6. Create a mutex for thread-safe output.

            7. For each thread index from 0 to num_threads - 1:
                a. Start a thread running the work function, passing the thread index and the mutex by reference.

            8. Wait for all threads to finish by joining each thread in the vector.
        */
    }
    catch (std::system_error &e)
    {
        std::cerr << "System error: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::invalid_option_value &e)
    {
        std::cerr << "Invalid option value: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::unknown_option &e)
    {
        std::cerr << "Unknown option: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::multiple_occurrences &e)
    {
        std::cerr << "Multiple occurrences of the same option: " << e.what() << "\n";
        return 1;
    }
    catch (std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
