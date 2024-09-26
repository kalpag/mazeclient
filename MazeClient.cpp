#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);

        // Connect to the server running on localhost at port 12345
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "12345"));

        // Loop to continuously accept and send commands
        while (true) {
            std::string command;
            std::cout << "Enter command (W/A/S/D for movement, 'find' to find nearest coin, 'kill' to stop): ";
            std::getline(std::cin, command);
            
            command += '\n';  // Append newline to send

            // Send the command to the server
            boost::asio::write(socket, boost::asio::buffer(command));

            // If the command is 'kill', exit the loop and stop the client
            if (command == "kill\n") {
                std::cout << "Shutting down client..." << std::endl;
                break;
            }

            // Read server's response, including the maze, until the "<END>" marker is found
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, "<END>\n");  // Read until the end marker

            // Output the server's response and maze
            std::istream response_stream(&buffer);
            std::string response_line;
            while (std::getline(response_stream, response_line)) {
                if (response_line == "<END>") break;  // Stop when the end marker is encountered
                std::cout << response_line << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
