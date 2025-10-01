/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @author Raghav Ramaswamy
 * @author Hari Thennarasu
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    srand(time(0));
    int current_time = 0;
    int context_save_time = 10;
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if(activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", " + "CPU burst\n";
            current_time += duration_intr;
        }

        else if (activity == "END_IO"){
            int randomNum = rand() % 200;
            execution += std::to_string(current_time) + ", " + std::to_string(randomNum) + ", " + "end of I/O " + std::to_string(duration_intr) + ": " + "interrupt\n";
            current_time += randomNum;
        }

        else if(activity == "SYSCALL"){
            auto [execution2, second_time] = intr_boilerplate(current_time, duration_intr, context_save_time, vectors);
            execution += execution2;
            current_time = second_time;
            execution += std::to_string(current_time) + ", " + std::to_string(delays[duration_intr]) + ", call device driver\n";
            current_time += delays[duration_intr];
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time++;
        }
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
