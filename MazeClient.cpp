#include <iostream>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class MazeClient {
public:
    MazeClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : socket_(io_context) {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket_, resolver.resolve(host, port));
    }

    void play() {
        while (true) {
            std::cout << "Enter command (W, S, A, D, path to nearest coin, exit to quit): ";
            std::string command;
            std::getline(std::cin, command);

            // Check if the player wants to exit the game
            if (command == "exit" || command == "quit") {
                std::cout << "Exiting the game. Goodbye!\n";
                break;  // Exit the game loop
            }

            // Send command to the server
            boost::asio::write(socket_, boost::asio::buffer(command + "\n"));

            // Read server response
            boost::asio::streambuf response;
            boost::asio::read_until(socket_, response, '\n');
            std::istream is(&response);
            std::string reply;
            std::getline(is, reply);
            std::cout << "Server reply: " << reply << std::endl;

            // If the response indicates victory, exit the loop
            if (reply == "Victory!\n") {
                std::cout << "Game over, you collected all the coins!" << std::endl;
                break;
            }
        }
    }

private:
    tcp::socket socket_;
};

int main() {
    try {
        std::cout << "Maze Client Running...." << std::endl;
        boost::asio::io_context io_context;
        MazeClient client(io_context, "127.0.0.1", "12345");
        client.play();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
