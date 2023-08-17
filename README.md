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

## Contributing

Niteen Pawar
