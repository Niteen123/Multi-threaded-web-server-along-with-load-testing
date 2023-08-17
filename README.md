# Simple HTTP Web Server

This is a simple HTTP web server implemented in C++ that can handle HTTP GET requests for serving HTML files and dynamic content.

## Features

- Handles HTTP GET requests for serving HTML files
- Supports dynamic HTTP responses for specific URLs
- Uses a thread pool for efficient handling of multiple clients

## Prerequisites

- C++ compiler
- POSIX Threads (Pthreads) library

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/simple-http-server.git
   cd simple-http-server
Compile the server:

bash
Copy code
g++ -o server http_server.cpp main.cpp -lpthread
Run the server:

bash
Copy code
./server
Open a web browser and navigate to http://localhost:8080 to test the server.

Configuration
You can modify the root directory for HTML files by editing the ROOT_DIRECTORY constant in http_server.h.
Customize the dynamic content handling logic in the handle_request function in http_server.cpp.
Dynamic Content
To support dynamic responses, follow these steps:

Implement dynamic content handling logic in the handle_request function based on the URLs you want to handle.
Update the response body to include the dynamic content.
Example:

/hello?name=John responds with "Hello John!"
/fib?n=10 responds with the 10th Fibonacci number
Contributing
Contributions are welcome! Feel free to open issues and pull requests.

