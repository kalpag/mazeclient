#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

class MazeClient {
public:
    MazeClient(const std::string& host, const std::string& port) 
        : resolver_(io_context_), socket_(io_context_) {
        connect(host, port);
    }

    void run() {
        std::thread readThread([this]() { read_from_server(); });
        std::thread inputThread([this]() { send_commands(); });

        readThread.join();
        inputThread.join();
    }

private:
    void connect(const std::string& host, const std::string& port) {
        boost::asio::connect(socket_, resolver_.resolve(host, port));
        std::cout << "Connected to the maze server." << std::endl;
    }

    void send_commands() {
        std::string command;
        while (true) {
            std::cout << "Enter command (W, A, S, D or 'exit' to quit): ";
            std::getline(std::cin, command);

            if (command == "exit") {
                break;
            }

            command += '\n'; // Add newline character for the server to recognize the end of input
            boost::asio::write(socket_, boost::asio::buffer(command));

            // Add a short sleep to prevent spamming the server
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        socket_.close();
    }

    void read_from_server() {
        try {
            while (true) {
                boost::asio::streambuf buffer;
                boost::asio::read_until(socket_, buffer, '\n');

                std::istream is(&buffer);
                std::string response;
                std::getline(is, response);
                std::cout << response << std::endl;

                // Check if the maze update has multiple lines
                while (std::getline(is, response)) {
                    std::cout << response << std::endl;
                }
            }
        } catch (std::exception& e) {
            std::cerr << "Exception in read_from_server: " << e.what() << std::endl;
        }
    }

    boost::asio::io_context io_context_;
    tcp::resolver resolver_;
    tcp::socket socket_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: MazeClient <host> <port>\n";
            return 1;
        }

        MazeClient client(argv[1], argv[2]);
        client.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

