#include <iostream>
#include <boost\array.hpp>
#include <boost\asio.hpp>

#include "json.hpp"
#include "minesweeper.h"

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;

void client()
{
	try
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);

		tcp::resolver::query query("localhost", "1234");

		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);

		boost::asio::connect(socket, endpoint_iterator);

		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;
			size_t len = socket.read_some(boost::asio::buffer(buf), error);


			if (error == boost::asio::error::eof) break;
			else if (error)
				throw boost::system::system_error(error);

			boost::system::error_code ignored_error;
			std::string message = "Received!";
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void startGamefunction(int &col, int &row, int &mineNum)
{
	try
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socket(io_service);

		boost::asio::connect(socket, endpoint_iterator);

		// write
		boost::system::error_code ignored_error;

		cout << "Input level: ";
		string message;
		getline(cin, message);
		if (message != "1" && message != "2" && message != "3")
		{
			cout << "Not supported level" << endl;
			cout << "Input level:";
			getline(cin, message);
		}

		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		cout << "Message send" << endl;

		// read
		cout << "Reading messages..." << endl;
		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
			throw boost::system::system_error(error);
		auto message_rec = string(buf.begin(), buf.begin()+len);
		auto message_rec_json = json::parse(message_rec);
		col = message_rec_json["col"];
		row = message_rec_json["row"];
		mineNum = message_rec_json["mineNum"];
		cout << "Message read: " << message_rec << endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void firstScriptTransfer(int col, int row, nlohmann::json& info)
{
	try
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socket(io_service);

		boost::asio::connect(socket, endpoint_iterator);

		// write
		boost::system::error_code ignored_error;

		json to_send;
		to_send["col"] = int(col / 2);
		to_send["row"] = int(row / 2);
		to_send["command"] = string("E");

		string message = to_send.dump();
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		cout << "Message send" << endl;

		// read
		cout << "Reading messages..." << endl;
		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
			throw boost::system::system_error(error);
		auto message_rec = string(buf.begin(), buf.begin() + len);
		info = json::parse(message_rec);
		cout << "Message read: " << message_rec << endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void ingameTransfer(nlohmann::json* to_send, nlohmann::json* info, MineSweeper* m)
{
	try
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socket(io_service);

		boost::asio::connect(socket, endpoint_iterator);

		// write
		boost::system::error_code ignored_error;

		string message = to_send->dump();
		boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		cout << "Message send" << endl;

		// read
		cout << "Reading messages..." << endl;
		boost::array<char, 1000> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
			throw boost::system::system_error(error);
		auto message_rec = string(buf.begin(), buf.begin() + len);
		*info = json::parse(message_rec);
		cout << "Message read: " << message_rec << endl;
		m->update(*info);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}