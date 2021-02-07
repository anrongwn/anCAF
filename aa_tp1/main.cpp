#include "aa_strand.h"
#include "an_spawn.h"
#include "asio.hpp"
#include <asio/io_context.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
#include <thread>
#include <utility>

#include "an_test.h"

using asio::ip::tcp;

//
class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket) : socket_(std::move(socket)){
        
    }

    void start(){
        do_read();
    }
private:
    void do_read(){
        auto self(shared_from_this());//keep-alive

        //asio每次最多读 max_length 字节数，注意 max_length 的长度，以保证吞吐量，尽量减少io
		socket_.async_read_some(asio::buffer(data_, max_length), [this, self](const asio::error_code& ec, std::size_t lenght) {
			if (!ec){
                std::cout << "***async_read_some data=" << std::string(data_, lenght) << ", lenght=" << lenght << std::endl;

                do_write(lenght);
			} else {
				socket_.close();

				std::cerr << "***async_read_some data error, " << ec.message() << std::endl;
			}
		});
	}

	void do_write(std::size_t length){
        auto self(shared_from_this());//keep-alive

		asio::async_write(socket_, asio::buffer(data_, length), [this, self](const asio::error_code& ec, std::size_t length) {
			if (!ec){
				std::cout << "***async_write data=" << std::string(data_, length) << ", length=" << length << std::endl;

				do_read();
			} else {
				socket_.close();

				std::cerr << "***async_write data error, " << ec.message() << std::endl;
			}
		});
	}
private:
    tcp::socket socket_;
    enum{max_length=65536};//保证足够的吞吐，但占用内存。
	char data_[max_length];

	//std::array<char, max_length> data2_;
};


class echo_service{
public:
  explicit echo_service(const std::string &addr, short port) : io_(2), acceptor_(io_), signals_(io_) {

	  signals_.add(SIGTERM);
	  signals_.add(SIGPIPE);
	  signals_.add(SIGINT);

	  do_await_stop();

	  //
	  asio::ip::address add;
	  add.from_string(addr);
	  tcp::endpoint ep(add, port);
	  acceptor_.open(ep.protocol());
	  acceptor_.set_option(asio::socket_base::reuse_address(true));
	  acceptor_.bind(ep);
	  acceptor_.listen();

	  do_accept();
  }
	echo_service()=delete;
	echo_service(const echo_service &) = delete;
	echo_service &operator=(const echo_service &) = delete;
	echo_service(echo_service &&) = delete;
	echo_service &operator=(echo_service &&) = delete;

	asio::io_context::count_type run() { return io_.run(); }

	void signal_handler(const asio::error_code &ec, int signal_num) {
		if (!ec) {
			switch (signal_num) {
			case SIGINT:
				std::cout << "signal_handler reciv SIGINT(" << signal_num << "),exit." << std::endl;
				acceptor_.close();
				io_.stop();
				// exit(-1);
				break;
			case SIGTERM:
				std::cout << "signal_handler reciv SIGTERM(" << signal_num << "),exit." << std::endl;
				acceptor_.close();
				io_.stop();
				// exit(-1);
				break;
			case SIGPIPE:
				std::cout << "signal_handler reciv SIGPIPE(" << signal_num << "),ignore." << std::endl;
				break;
			default:
				std::cout << "signal_handler reciv other(" << signal_num << "),exit." << std::endl;
				acceptor_.close();
				io_.stop();
				exit(-1);
				break;
			}
		}
	}

private:
    void do_accept(){
		acceptor_.async_accept([this](const asio::error_code &ec, tcp::socket socket) {
			if (!acceptor_.is_open()) {
				return;
			}

			if (!ec){
                auto peer =socket.remote_endpoint();
                std::cout<<"=====echo_service accept from: " << peer.address() << ":" << peer.port() << "(" << peer.protocol().type() <<")."<<std::endl;

				std::make_shared<session>(std::move(socket))->start(); //启动当前session读写

				do_accept();
			} else {
				std::cerr << "=====echo_service accept error, " << ec.message() << std::endl;
			}
		});
	}

	void do_await_stop() {
		signals_.async_wait(
			std::bind(&echo_service::signal_handler, this, std::placeholders::_1, std::placeholders::_2));
	}

  private:
	asio::io_context io_;
	tcp::acceptor acceptor_;
	asio::signal_set signals_;
};

struct Task
{
    void doSomething(int task_type)
    {
        std::cout<<"task_type:"<<task_type<<std::endl;
    }
};

/*//test 
// gtest unit test
TEST(FactorialTest, Negative) {
	EXPECT_EQ(1, Factorial(-5));
	EXPECT_EQ(1, Factorial(-1));
	EXPECT_GT(Factorial(-10), 0);
}
TEST(FactorialTest, Zero) { EXPECT_EQ(1, Factorial(0)); }

TEST(IsPrimeTest, Negative) {
  // This test belongs to the IsPrimeTest test case.

  EXPECT_FALSE(IsPrime(-1));
  EXPECT_FALSE(IsPrime(-2));
  EXPECT_TRUE(IsPrime(-3));
  EXPECT_FALSE(IsPrime(INT_MIN));
}

TEST(str2strTest, comp) { ASSERT_STREQ(str2str("wang", "jr").c_str(), "wang jr"); }

class anEv : public ::testing::Environment {
  public:
	anEv() { std::cout << "anEv::anEv() " << std::endl; }
	~anEv() { std::cout << "anEv::~anEv() exit" << std::endl; }
	virtual void SetUp() override { std::cout << "anTest Ev SetUp..." << std::endl; }

	virtual void TearDown() override { std::cout << "anTest Ev TearDown()." << std::endl; }
};

TEST(anDataTest, conf) {
	anData *p = new anData();

	// ASSERT_EQ(p, nullptr);
	ASSERT_TRUE(p != nullptr);

	// EXPECT_EQ(p->push(""), static_cast<std::size_t>(1)); // failure
	EXPECT_EQ(p->push(""), static_cast<std::size_t>(0));
	EXPECT_EQ(p->push("A"), static_cast<std::size_t>(1));
	EXPECT_EQ(p->push("2"), static_cast<std::size_t>(1));
	EXPECT_EQ(p->push("3"), static_cast<std::size_t>(1));
	EXPECT_EQ(p->push("D"), static_cast<std::size_t>(1));

	// EXPECT_EQ(p->get_size(), 10);//--failure
	EXPECT_EQ(p->get_size(), static_cast<std::size_t>(4));

	delete p;
}
*/

// test fixture
TEST_F(anDataTest, getsize) {
	ASSERT_TRUE(p_data_ != nullptr);

	EXPECT_EQ(p_data_->get_size(), static_cast<std::size_t>(0));
}

TEST_F(anDataTest, push) {
	// anData *p = new anData();

	ASSERT_TRUE(p_data_ != nullptr);

	EXPECT_EQ(p_data_->push("A"), static_cast<std::size_t>(1));
	// delete p;
}

int main(int argc, char *argv[]) {
	//
	// testing::AddGlobalTestEnvironment(new anEv);

	//::testing::InitGoogleTest();
	testing::InitGoogleTest(&argc, argv); // --help

	return RUN_ALL_TESTS();

	::std::string_view good("a string literal");
	std::string str(good);

	std::cout << "wangjr, " << str << std::endl;
	if (argc != 3) {
		std::cerr << "Usage: ./aa_tp1 <address> <port>\n";
		return 1;
	}

	try {

		/***
		// asio::io_context io(1);

		// asio::signal_set signals(io, SIGINT, SIGTERM, SIGPIPE);
		// signals.add(SIGTERM);
		// signals.add(SIGPIPE);
		// signals.add(SIGINT);

		echo_service svr(std::string(argv[1]), std::atoi(argv[2]));

		// signals.async_wait(
		//std::bind(&echo_service::signal_handler, &svr, std::placeholders::_1, std::placeholders::_2));

		// io.run();
		svr.run();
		***/

		/**coroutine
		asio::io_context io(2);

		asio::spawn(io, [&](asio::yield_context yield) {
			std::cout << std::this_thread::get_id() << " acceptor spawn begin" << std::endl;
			tcp::acceptor accepter(io, tcp::endpoint(tcp::v4(), std::atoi(argv[2])));
			for(;;){
				std::cout << std::this_thread::get_id() << " acceptor spawn loop" << std::endl;
				asio::error_code ec;
				tcp::socket ss(io);
				accepter.async_accept(ss, yield[ec]); // 1
				if (!ec){
					std::cout << std::this_thread::get_id() << " async_accept  " << std::endl;
					std::make_shared<spawn_session>(io, std::move(ss))->go(); // 2
				}
			}
		});

		io.run();
		**/

		std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;
		asio::io_context io;
		printer p(io);
		std::thread t([&io]() {
			std::cout << "sub thread id = " << std::this_thread::get_id() << std::endl;
			io.run();
		});

		//Task task1;
		//std::thread t(std::bind(&Task::doSomething, &task1, 2));
		std::this_thread::sleep_for(std::chrono::microseconds(2000));

		io.run();
		
		t.join();

	} catch (std::exception &e) {
		std::cerr << "Exception : " << e.what() << std::endl;
        return 1;
	}

	return 0;
}
