Fake OS Simulation Project
Description
The project implements a simulation of a fictitious operating system (Fake OS). The system includes the management of processes, CPU bursts, and IO bursts. The implemented scheduler is a round-robin scheduler on multiple cores.

Requirements
GCC Compiler (GNU Compiler Collection)
Linux or UNIX-like operating system
Compilation
To compile the program, execute the following command in the project directory:

c
Copy code
gcc -o sched_sim linked_list.c fake_os.c fake_process.c sched_sim.c -std=gnu99 -Wall
Execution
The program accepts input files specifying the sequence of events for each process. Example input file:

Copy code
PROCESS 1 0
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
IO_BURST 30
CPU_BURST 1
To run the program, use the following command:

php
Copy code
./sched_sim <file_process1> <file_process2> ... <file_processN>
Where <file_process1>, <file_process2>, ..., <file_processN> are the input files for individual processes.

Testing
In the test folder, there are input files for testing. To run the tests, use the following command:

bash
Copy code
./sched_sim test/*
Technical Specifications
The operating system simulator is implemented in the C programming language.
Linked list management is implemented in the linked_list.c library.
The main program sched_sim.c initializes the fictitious operating system, loads processes, and starts the simulation.
The schedulers are implemented in the schedRR function in sched_sim.c using a round-robin algorithm.
The time quantum for each core can be adjusted in the srr_args.quantum variable in sched_sim.c.
Process input files follow a specific format, as shown in the test examples.
For further details on the code's functionality, refer to the source code in the linked_list.c, fake_os.c, fake_process.c, and sched_sim.c files.
