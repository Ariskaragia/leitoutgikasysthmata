🍕 Pizza Shop Simulation (C + pthreads)

** Description

- Implemented a pizzeria simulation in C where each customer/order runs as a separate thread (pthreads) and competes for shared resources (phone lines, cooks, ovens, deliverers). 
- Designed synchronization mechanisms using mutexes and condition variables for safe resource allocation and thread coordination. 
- Simulated stochastic service times (order, preparation, baking, delivery) and collected system statistics (successful/failed orders, mean/max service and cooling times). 
- Identified critical concurrency issues (race conditions, out-of-bounds indexing, division by zero) and proposed fixes to improve correctness and scalability. 

A multithreaded C program that simulates a pizza shop workflow using POSIX threads (pthreads).
Each thread represents a customer placing an order and competing for limited shop resources (phone lines, cooks, ovens, and delivery staff). The program measures and prints basic performance statistics (service time, cooling/delivery time, successful/failed orders).

✅ What it simulates

A customer arrives and places an order after a random ordering time.

The order requires a random number of pizzas.

The system allocates resources in sequence:

Telephone line

Cook

Oven

Deliverer

The order goes through preparation/baking/packing and then delivery.

At the end, the program prints aggregated statistics.

🧩 Project Structure

c_ergas.c — main implementation (threads, synchronization, simulation logic)

c_ergas.h — constants & structures (resource counts, time ranges, customer struct)

🛠️ Requirements

GCC (or Clang)

POSIX threads (pthread)

Linux / macOS (or Windows via WSL)

▶️ Build & Run
Compile
gcc -O2 -Wall -Wextra -pthread c_ergas.c -o pizza_sim

Run
./pizza_sim

📊 Output (example)

The program typically prints per-order messages (prepared/delivered) and then overall statistics, such as:

Total successful / failed orders

Total “sales” (depending on implementation)

Mean and max service time

Mean and max cooling/delivery time

🔒 Synchronization

The simulation uses:

pthread_mutex_t for protecting shared state

pthread_cond_t to block threads until resources become available

⚠️ Notes / Known Issues (optional section)

Depending on your current implementation, you may want to check:

Thread-safe updates on shared counters/arrays (use mutex)

Array indexing (0-based vs 1-based customer IDs)

Division-by-zero when there are 0 successful orders

🚀 Future Improvements (ideas)

Use real pricing (per pizza type) and compute revenue accurately

Make ovens depend on number of pizzas (e.g., multiple oven slots)

Add a command-line interface (e.g., ./pizza_sim <Ncustomers>)

Add logging timestamps and more detailed performance metrics

📄 License

Choose one:

MIT License

Apache-2.0

Or leave unspecified
