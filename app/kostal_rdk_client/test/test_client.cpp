#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <jsoncpp/json/json.h>

using boost::asio::ip::tcp;

enum
{
    max_length = 1024
};

int main(int argc, char* argv[])
{
    try {
        // Init
        //====================================
        boost::asio::io_context io_context;
        tcp::socket s1(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(s1, resolver.resolve("127.0.0.1", "6070"));

        // Write
        std::string stringtemp = "";
        Json::Value config;
        config["CPOL"] = "0";
        config["CPHA"] = "1";
        config["LSB"] = "0";
        config["SELP"] = "0";
        config["TOKEN"] = "kostal";
        stringtemp = Json::FastWriter().write(config);
        std::cout << "The client's init msg is: " << stringtemp;
        const char* request = stringtemp.c_str();
        boost::asio::write(
            s1, boost::asio::buffer(request, std::strlen(request)));

        // Read
        char reply[max_length];
        boost::system::error_code ec;
        size_t read_length
            = s1.read_some(boost::asio::buffer(reply, max_length), ec);
        std::string m_recvMsg = "";
        for (int i = 0; i < read_length; i++) {
            m_recvMsg += reply[i];
        }

        std::cout << "The server's reply is: " << m_recvMsg << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Shake-hand
        //===========================================================
        // Write
        Json::Value config2;
        config2["TM_FLEXIV_QUERY_STATUS"] = "true";
        config2["TM_FLEXIV_TERMINATE"] = "false";
        config2["TM_FLEXIV_TASK_TYPE"] = "NORMAL";
        config2["TM_FLEXIV_TASK_NAME"] = "Kostal-MainPlan";
        stringtemp = Json::FastWriter().write(config2);
        request = stringtemp.c_str();
        int i = 3;
        while (i > 0) {
            boost::asio::write(
                s1, boost::asio::buffer(request, std::strlen(request)));
            std::cout << "The client task msg is: " << stringtemp << std::endl;

            // Read
            read_length
                = s1.read_some(boost::asio::buffer(reply, max_length), ec);
            m_recvMsg = "";
            for (int i = 0; i < read_length; i++) {
                m_recvMsg += reply[i];
            }

            std::cout << "The server's reply is: " << m_recvMsg << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            i--;
        }

        // Task
        //=========================================
        config2["TM_FLEXIV_QUERY_STATUS"] = "false";
        stringtemp = Json::FastWriter().write(config2);
        request = stringtemp.c_str();

        // Write
        boost::asio::write(
            s1, boost::asio::buffer(request, std::strlen(request)));
        std::cout << "The client task msg is: " << stringtemp << std::endl;

        // Read
        read_length = s1.read_some(boost::asio::buffer(reply, max_length), ec);
        m_recvMsg = "";
        for (int i = 0; i < read_length; i++) {
            m_recvMsg += reply[i];
        }
        std::cout << "The server's reply is: " << m_recvMsg << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check
        //===========================================================
        // Write
        config2["TM_FLEXIV_QUERY_STATUS"] = "true";
        stringtemp = Json::FastWriter().write(config2);
        request = stringtemp.c_str();
        i = 20;
        while (i > 0) {
            boost::asio::write(
                s1, boost::asio::buffer(request, std::strlen(request)));
            std::cout << "The client task msg is: " << stringtemp << std::endl;

            // Read
            read_length
                = s1.read_some(boost::asio::buffer(reply, max_length), ec);
            m_recvMsg = "";
            for (int i = 0; i < read_length; i++) {
                m_recvMsg += reply[i];
            }

            std::cout << "The server's reply is: " << m_recvMsg << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            i--;
        }

        // Done
        //=========================================
        config2["TM_FLEXIV_TERMINATE"] = "true";
        config2["TM_FLEXIV_QUERY_STATUS"] = "false";
        stringtemp = Json::FastWriter().write(config2);
        request = stringtemp.c_str();

        // Write
        boost::asio::write(
            s1, boost::asio::buffer(request, std::strlen(request)));
        std::cout << "The client task msg is: " << stringtemp << std::endl;

        // Read
        read_length = s1.read_some(boost::asio::buffer(reply, max_length), ec);
        m_recvMsg = "";
        for (int i = 0; i < read_length; i++) {
            m_recvMsg += reply[i];
        }
        std::cout << "The server's reply is: " << m_recvMsg << std::endl;

        // wait for close
        std::this_thread::sleep_for(std::chrono::seconds(5));

        s1.close();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}