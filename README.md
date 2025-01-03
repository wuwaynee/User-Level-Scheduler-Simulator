# User-Level Scheduler Simulator

A user-level thread scheduler simulator that implements three scheduling algorithms (FCFS, RR, Priority Preemptive) and simulates resource allocation and signal handling using the Linux `ucontext` library.

## Features
- **Scheduling Algorithms**:
  - FCFS (First-Come-First-Serve)
  - RR (Round Robin) with time quantum
  - Priority-based Preemptive Scheduling
- **Thread Management**:
  - Supports task creation, termination, and state transitions (READY, RUNNING, WAITING, TERMINATED).
- **Resource Management**:
  - Simulates 8 resources with APIs for allocation and release.
- **Timer and Signal Handling**:
  - Periodic scheduler invocation using `setitimer` and `SIGVTALRM`.
- **Command-Line Interface**:
  - Interactive commands: `add`, `del`, `ps`, `start`, and more.

## Input and Output
- **Input**: Command-line instructions for thread and resource management.
- **Output**: Logs task states, scheduling decisions, and resource usage.

---

## Usage

### Compilation
1. Clean and compile the program:
   ```bash
   make clean
   make

### Execution
1. Run the scheduler simulator with a specified algorithm:
   ```bash
   ./scheduler_simulator <scheduling_algorithm>
- `<scheduling_algorithm>` can be `FCFS`, `RR`, or `PP`.

### Commands
- Add a task:
   ```bash
   add <task_name> <function_name> <priority>
- Delete a task:
   ```bash
   del <task_name>
- Show task states:
   ```bash
   ps
- Start the simulation:
   ```bash
   start

---

## Testing

### Auto Test
1. Run the auto test script:
   ```bash
   python3 test/auto_run.py all test/general.txt

### Manual Test
1. Test commands:
   - Example commands:
   ```bash
   add aaa task1 1
   add bbb task2 2
   del aaa
   ps
2. Test algorithms:
   - Manually test each algorithm (FCFS, RR, Priority) with `test/general.txt`.
   - Compare results with the provided reference output.
3. Test pause and resume:
   - Start the simulation, use `Ctrl+Z` to pause, and verify the system state with `ps`.
























