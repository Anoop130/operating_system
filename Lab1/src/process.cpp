#include "process.hpp"
#include "ForkException.hpp"

#include <iostream>


namespace cse4733
{

    process::process(std::shared_ptr<ISystemCalls> systemCalls,
                     std::string path,
                     const std::vector<std::string> &args)
        : processId(-1),
          executablePath(std::move(path)),
          sysCalls(std::move(systemCalls)),
          errorMessage("(none)")
    {

        for (const auto &arg : args)
        {
            arguments.push_back(const_cast<char *>(arg.c_str()));
        }
        arguments.push_back(nullptr);
    }

    auto process::execute() -> bool
    {
        std::optional<int> pid = sysCalls->Fork();

        if (pid < 0) {
            std::cerr << "Fork failed" << std::endl;
            std::cout << "Error: " << getErrorMessage() << std::endl;
            return false;
        } else if (pid == 0) {
            executeChildProcess();
            return false;
        } else {
            executeParentProcess(pid.value());
            return true;
        }

        // TODO:
        // Call the system fork function to create a child process
        // IF the fork fails, display an error message and return false
        // IF the fork succeeds, check if this is the child process or the parent process
        //   IF the child:
        //     Execute the child process logic
        //     Return false to indicate that this is the child process
        //   ELSE the parent:
        //     Execute the parent process logic
        //     Return true to indicate that this is the parent process and the child process was created successfully
    }

    void process::executeChildProcess()
    {
        std::cout << "child process ID: " << sysCalls->Getpid() << std::endl;
        std::cout << "parent process ID: " << sysCalls->Getppid() << std::endl;

        sysCalls->Execvp(executablePath.c_str(), arguments);
        std::cerr << "Execvp failed" << std::endl;
        exit(1);


        // TODO:
        //   Print the child process ID
        //   Print the parent process ID
        //   Execute the given program with the provided arguments
        //   Should Execvp return (if it does?) then print an error message and exit
    }

    void process::executeParentProcess(pid_t pid)
    {
        this->processId = pid;
        std::cout << "parent process ID: " << sysCalls->Getpid() << std::endl;
        std::cout << "child process ID: " << pid << std::endl;
        // TODO:
        //   Store the child process ID
        //   Print the parent process ID
        //   Print the child process ID
    }

    void process::handleChildExitStatus(int status, pid_t pid)
    {
        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Child process " << pid << " terminated normally with exit status " << exitStatus << std::endl;
        } else {
            std::cerr << "Child process " << pid << " did not terminate normally" << std::endl;
        }
        // TODO:
        // Check if the child process terminated normally
        // IF the child process terminated normally
        //    Print a message with the child process ID with the exit status.
        // ELSE IF the child process did not terminate normally
        //    Print an error message with the child process ID
    }

    auto process::wait() -> std::optional<int>
    {
        int status;
        std::optional<pid_t> result = sysCalls->Waitpid(this->processId, &status, 0);
        if (!result) {
            std::cerr << "Waitpid failed" << std::endl;
            return std::nullopt;
        } else {
            handleChildExitStatus(status, this->processId);
            return WIFEXITED(status);
        }
        // Declare a variable to hold the status of the child process
        // Use the Waitpid system call to wait for the child process to change state (e.g., to terminate)
        // Check if the Waitpid system call succeeded
        // If the Waitpid system call failed, print an error message and return std::nullopt
        // Else if the Waitpid system call succeeded, handle the child's exit status
        // Return the exit status of the child process
        return std::nullopt;
    }

    auto process::get_id() const -> pid_t
    {
        return this->processId;
    }

    auto process::getErrorMessage() const -> std::string
    {
        return errorMessage;
    }

} // namespace cse4733
