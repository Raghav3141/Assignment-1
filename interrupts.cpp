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
    int current_time = 0;
    int context_save_time = 10;
    int isr_activity = 40;
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
            int delay_time = delays[duration_intr];
            
            auto [execution2, second_time] = intr_boilerplate(current_time, duration_intr, context_save_time, vectors);
            execution += execution2;
            current_time = second_time;
            
            int minTime = (delay_time > isr_activity) ? isr_activity : delay_time;

            execution += std::to_string(current_time) + ", " + std::to_string(minTime) + ", call device driver\n";
            current_time += minTime;
            delay_time -= minTime;
            
            minTime = (delay_time > isr_activity) ? isr_activity : delay_time;
            
            execution += std::to_string(current_time) + ", " + std::to_string(minTime) + ", run END_IO ISR\n";
            current_time += minTime;
            delay_time -= minTime;
            
            execution += std::to_string(current_time) + ", " + std::to_string(delay_time) + ", finish END_IO ISR\n";
            current_time += delay_time;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time++;
        }

        else if(activity == "SYSCALL"){
            int delay_time = delays[duration_intr];

            auto [execution2, second_time] = intr_boilerplate(current_time, duration_intr, context_save_time, vectors);
            execution += execution2;
            current_time = second_time;

            int minTime = (delay_time > isr_activity) ? isr_activity : delay_time;

            execution += std::to_string(current_time) + ", " + std::to_string(minTime) + ", call device driver\n";
            current_time += minTime;
            delay_time -= minTime;

            execution += std::to_string(current_time) + ", " + std::to_string(delay_time) + ", finish SYSCALL ISR\n";
            current_time += delay_time;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time++;
        }
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
