#include <cstdint>
#include <evhttp.h>
#include <iostream>
#include <memory>

/**
 * Event-based, single-threaded Web Server program using evhttp.h.
 * Modified version of http://kukuruku.co/hub/cpp/lightweight-http-server-in-less-than-40-lines-on-libevent-and-c-11
 */
int main() {
	// libevent initialization
	if ( !event_init() ) {
		std::cerr << "Failed to init libevent." << std::endl;
		return -1;
	}

	char const SrvAddress[] = "127.0.0.1";
	std::uint16_t SrvPort = 5555;
	std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);

	if (!Server) {
		std::cerr << "Failed to init http server." << std::endl;
		return -1;
	}

	void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *)
	{
		auto *OutBuf = evhttp_request_get_output_buffer(req);
		if (!OutBuf)
		return;
		evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello World!</h1></center></body></html>");
		evhttp_send_reply(req, HTTP_OK, "", OutBuf);
	};

	evhttp_set_gencb(Server.get(), OnReq, nullptr);
	if (event_dispatch() == -1) {
		std::cerr << "Failed to run message loop." << std::endl;
		return -1;
	}
	std::cout << "if you see this, event dispatch is async" << std::endl;

	return 0;
}
