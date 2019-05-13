#pragma once

#include <string_view>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <lwipstack.h>
#include <tun2socks.h>
#include <cstdio>
#include <cstring> // for memcpy
#include <map>

namespace tun2socks {
	inline std::string get_address_string(u32_t ip) {
		char buf[160];
		snprintf(buf, 16, "%d.%d.%d.%d", ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
		return std::string(buf);
	}

	enum TUNSTATE {
		CLOSE = 0,
		OPEN,
		OPEN_FAILURE
	};

#ifdef __WINDOWS__
	struct Request {
		OVERLAPPED overlapped;
		pbuf* buf;
		DWORD transfered;
	};
#endif
#ifdef __LINUX__
	struct Request{
	    pbuf* buf;
	    int transfered;
	};
#endif

	class TUNDevice {
		
	public:
		TUNDevice(boost::asio::io_context&, const TUNAdapter&);

		int tap_set_address();

		void start_read(std::function<void(std::shared_ptr<Request>)>, std::function<void(const boost::system::error_code&)>);

		void do_write(std::unique_ptr<u_char[]>&&, size_t, std::function<void()>, std::function<void(const boost::system::error_code&)>);


	private:
		TUNHANDLE _tun_handle;
		TUNAdapter _adapter;
		boost::asio::io_context& _ctx;
	};
}