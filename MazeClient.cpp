#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace boost;

int main() {
    try {
        asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        asio::connect(socket, resolver.resolve("127.0.0.1", "1234"));

        std::cout << "Connected to the server!\n";

        // Receive welcome message
        char data[128];
        size_t length = socket.read_some(asio::buffer(data));
        std::cout << std::string(data, length) << std::endl;

        while (true) {
            // Get player input (WASD for movement)
            char move;
            std::cin >> move;

            asio::write(socket, asio::buffer(&move, 1));

            // Read server response
            length = socket.read_some(asio::buffer(data));
            std::cout << std::string(data, length) << std::endl;
        }

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
