/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int time = 0;
    


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        std::vector<std::string> tokens = split_delim(trace, ", ");
        if (tokens[0] == "CPU")
        {
            execution += (std::to_string(time) + ", " + tokens[1] + ", CPU Execution\n");
            time += std::stoi(tokens[1]);
        }
        else
        {
            if (tokens[0] == "SYSCALL")
            {
                /*I don't know how we can make every execution step here 40ms*/
                execution += (std::to_string(time) + ", 1, switch to kernel mode\n");
                time += 1;
                execution += (std::to_string(time) + ", 10, context saved\n");
                time +=10;
                execution += (std::to_string(time) + ", 1, find vector " + tokens[1] + " in memory position " + std::to_string((std::stoi(tokens[1]))*2)+"\n");
                time += 1;
                execution += (std::to_string(time) + ", 1, obtrain ISR address\n");
                time +=1;
                /*I don't know how we can make every execution step here 40ms, as the time the devices take, listed in the device table, are not all multiples of 40*/
                /*I would love to hear about how this can be done, however I am implimenting it like this for now, despite the fact I know it may be wrong.*/
                /*The insturctions for this assignment are very unclear, so I applogize in advance if this is wrong, I am simply trying to understand the instructions the best I can.*/

                std::ifstream device_table(argv[3]);
                std::string line;
                int current_line = 1;   
                while (std::getline(device_table, line)) {
                    if (current_line == std::stoi(tokens[1])) {
                        break;
                }
                    current_line++;
                }
                device_table.close();
                int device_driver_time = std::stoi(line)/4;
                int execute_time = std::stoi(line) - device_driver_time - 10;

                execution += (std::to_string(time) + ", " + std::to_string(device_driver_time) +", call device driver\n");
                time += device_driver_time;
                execution += (std::to_string(time) + ", " + std::to_string(execute_time) + ", execute interrupt\n");
                time += execute_time;
                execution += (std::to_string(time) + ", 10, context restored\n");
                time +=10;
            }
            else
            {
                if (tokens[0] == "END_IO")
                {
                    execution += (std::to_string(time) + ", 1, IRET\n");
                    time +=1;
                                    std::ifstream device_table(argv[3]);
                    std::string line;
                    int current_line = 1;   
                    while (std::getline(device_table, line)) {
                        if (current_line == std::stoi(tokens[1])) {
                            break;
                        }
                    current_line++;
                    }
                    device_table.close();
                    execution += (std::to_string(time) + "," + line + ", end of I/O "+ tokens[1] +"\n");
                    time += std::stoi(line);

                }
            }

        }
        

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
