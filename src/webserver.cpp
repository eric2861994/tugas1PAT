#include <evhttp.h>
#include <iostream>
#include <memory>

#include "property.h"


const std::string CONFIG_FILE = "config.prop";


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

	// load configuration file
	Property config(CONFIG_FILE);

	// setup server
	char const SrvAddress[] = "localhost";
	std::uint16_t SrvPort = std::stoi(config.get("port"));
	std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);

	if (!Server) {
		std::cerr << "Failed to setup http server." << std::endl;
		return -1;
	}

	// setup handler function
	void (*OnReq)(evhttp_request *req, void *) = [] (evhttp_request *req, void *) {
		// get output buffer
		// TODO do we need to dealocate?
		auto *OutBuf = evhttp_request_get_output_buffer(req);
		if (!OutBuf)
			return;
		std::cout << evhttp_request_get_uri(req) << std::endl;
		std::string out = "<html><body><center><h1>Hello World!</h1></center></body></html>";

		// determine
		// TODO find out if it is blocking or not
		evbuffer_add_printf(OutBuf, out.c_str());
		evhttp_send_reply(req, HTTP_OK, "", OutBuf);
	};

	// dispatch server
	// TODO what is the third parameter for?
	evhttp_set_gencb(Server.get(), OnReq, nullptr);
	if (event_dispatch() == -1) {
		std::cerr << "Failed to run message loop." << std::endl;
		return -1;
	}

	return 0;
}
