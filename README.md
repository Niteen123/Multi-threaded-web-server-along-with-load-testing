# Simple HTTP Web Server

This project implements a simple HTTP web server in C++ that can handle HTTP GET requests for serving HTML files and dynamic content.

## Features

- Handles HTTP GET requests for serving HTML files
- Supports dynamic HTTP responses for specific URLs
- Utilizes a thread pool for efficient client handling

## Prerequisites

- C++ compiler
- POSIX Threads (Pthreads) library

## Installation and Usage

1. **Clone the repository:**

    ```sh
    git clone https://github.com/yourusername/simple-http-server.git
    cd simple-http-server
    ```

2. **Compile the server:**

    ```sh
    g++ -o server http_server.cpp main.cpp -lpthread
    ```

3. **Run the server:**

    ```sh
    ./server
    ```

4. **Test the server:**

    Open a web browser and navigate to `http://localhost:8080` to test the server.

## Configuration

- Modify the root directory for HTML files by editing the `ROOT_DIRECTORY` constant in `http_server.h`.
- Customize the dynamic content handling logic in the `handle_request` function in `http_server.cpp`.

## Dynamic Content

To support dynamic responses, follow these steps:

1. Implement dynamic content handling logic in the `handle_request` function based on the URLs you want to handle.
2. Update the response body to include the dynamic content.

For example:
- `/hello?name=John` responds with "Hello John!"
- `/fib?n=10` responds with the 10th Fibonacci number


# Load Testing Guide for Web Server Capacity

Load testing is a critical process that helps you evaluate the performance and capacity of your web server. By simulating real-world user loads, you can identify potential bottlenecks and ensure your server can handle the expected traffic.

## Why Load Testing?

Load testing helps you:

* Determine how well your web server performs under different levels of load.
* Identify bottlenecks and performance issues.
* Ensure your server can handle expected user loads without degradation.

## Prerequisites

Before you begin, ensure you have:

* A C++ compiler
* The POSIX Threads (Pthreads) library
* [Valgrind](http://valgrind.org/) for memory leak detection

## Step 1: Addressing Memory Leaks

Before load testing, ensure your web server code is free from memory leaks. Use Valgrind to identify and fix memory leaks that could affect performance over time.

## Step 2: Implementing Load Generator Client

1. Create a basic HTTP client that sends HTTP GET requests to your web server.
2. Expand the client to act as a closed-loop load generator:
    * Configure the number of concurrent users (threads).
    * Set think time (delay between requests) and duration of the load test.


## Step 3: Running the Load Test
1.Compile your web server and load generator.

g++ -o server http_server.cpp main.cpp -lpthread
g++ -o load_generator LoadGenerator.cpp -lpthread

2.Run your web server and the load generator on separate machines or separate CPU cores.

Start the server:

./server
## Run the load generator:
./load_generator <concurrent_users> <think_time> <duration>


3. Vary the load levels and think time for each test run.

    Experiment with different numbers of concurrent users and think time intervals to simulate various real-world scenarios.

4. Run each test for at least a minute to achieve steady-state results.

    Longer test durations help ensure that the server reaches a stable state and provides accurate performance metrics.

5. Monitor hardware resources (CPU, memory, disk) using tools like `top` or `iostat`.

    Observe the resource utilization of both the server and the load generator to identify potential bottlenecks.

## Step 4: Analyzing Results

After each load test run, gather and analyze the following performance metrics:

* Average throughput of the server (requests per second).
* Average response time of the server (time taken to receive a response).
* Identify hardware resources that hit close to 100% utilization (e.g., CPU, network, disk).

## Step 5: Plotting Performance Metrics

Generate plots of average throughput and response time as functions of the load level. These plots help you understand how the server's performance scales with increasing load.

## Step 6: Identifying Bottlenecks

Look for points where throughput plateaus and response time sharply increases. These are indicators of the server's capacity limits and potential bottlenecks.

## Conclusion

Load testing is essential to ensure your web server can handle expected user loads without performance issues. By identifying and addressing bottlenecks, you can optimize your server's performance and deliver a smooth user experience.



