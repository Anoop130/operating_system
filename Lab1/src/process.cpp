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
        std::optional<pid_t> pid = sysCalls->Fork();

        if (!pid.has_value()) {
            std::cerr << "Fork failed" << std::endl;
            errorMessage = "Failed to create child process.\n";
            std::cout << "Error: " << getErrorMessage() << std::endl;
            return false;
        } else if (pid.value() == 0) {
            executeChildProcess();
            return false;
        } else {
            executeParentProcess(pid.value());
            return true;
        }
    }

    void process::executeChildProcess()
    {
        std::cout << "[CHILD]: child process ID: " << sysCalls->Getpid() << std::endl;
        std::cout << "[CHILD]: parent process ID: " << sysCalls->Getppid() << std::endl;

        if (!sysCalls->Execvp(executablePath.c_str(), arguments)) {
            // if Execvp returns, it failed
            std::cerr << "Execvp failed" << std::endl;
            errorMessage = "Execvp failed\n";
            exit(1);
        }
    }

    void process::executeParentProcess(pid_t pid)
    {
        this->processId = pid;
        std::cout << "[PARENT]: parent process ID: " << sysCalls->Getpid() << std::endl;
        std::cout << "[PARENT]: child process ID: " << pid << std::endl;
    }

    void process::handleChildExitStatus(int status, pid_t pid)
    {
        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            std::cout << "Child process " << pid << " terminated normally with exit status " << exitStatus << std::endl;
        } else {
            std::cerr << "Child process " << pid << " did not terminate normally" << std::endl;
        }
    }

    auto process::wait() -> std::optional<int>
    {
        int status;
        std::optional<pid_t> result = sysCalls->Waitpid(this->processId, &status, 0);
        if (!result) {
            std::cerr << "Waitpid failed" << std::endl;
            return std::nullopt;
        } 
        handleChildExitStatus(status, this->processId);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        
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
