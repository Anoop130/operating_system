// SignalHandlers.cpp
#include <iostream>

#include "cse4733/GlobalVariables.hpp"
#include "cse4733/SignalHandlers.hpp"
#include "cse4733/SystemCalls.hpp" // Include any necessary headers

void handleSIGINT(int signal)
{
    // TODO: Implement the handleSIGINT function.
    // 1. Get the PID of the child process using the GetPid method from the SystemCalls class.
    // 2. Output a message to the console indicating that the child process received a SIGINT signal and is exiting.
    // 3. Exit the child process with a success status code (0).
}

void handleSIGUSR1(int signal)
{
    // TODO: Implement the handleSIGUSR1 function.
    // 1. Check if the global process manager object exists.
    // 2. If the global process manager object exists:
    //    2.1 Output a message to the console indicating that a SIGUSR1 signal has been received and a message is being logged.
    //    2.2 Call the incrementSleepValue method on the global process manager object.
    //    2.3 Output the increased thread sleep value, which is retrieved from the atomic variables object in the global process manager.
}

void handleSIGALRM(int signal)
{
   // TODO: Implement the handleSIGALRM function.
    // 1. Check if the global process manager object exists.
    // 2. If the global process manager object exists:
    //    2.1 Output a message to the console indicating that a SIGALRM signal has been received and a time-out action is being performed.
    //    2.2 If the sleep value in the global process manager object is greater than 1, 
    //    2.2.1 Call the decrementSleepValue method on the global process manager object.
    //    2.2.2 Output the decreased thread sleep value, which is retrieved from the atomic variables object in the global process manager.    

}