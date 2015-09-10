#include "property.h"

#include <evhttp.h>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <sys/stat.h>


const std::string CONFIG_FILE = "res/config.prop";


/**
 * Event-based, single-threaded Web Server program using evhttp.h.
 */
int main() {
	// libevent initialization
	if ( !event_init() ) {
		std::cerr << "Failed to init libevent." << std::endl;
		return -1;
	}

	// load configuration file
	Property config(CONFIG_FILE);
	std::cout << "Loaded config: " << CONFIG_FILE << std::endl;

	// setup server
	char const SrvAddress[] = "localhost";
	std::uint16_t SrvPort = std::stoi(config.get("port"));
	std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
	if (!Server) {
		std::cerr << "Failed to setup http server." << std::endl;
		return -1;
	}
	std::cout << "Server run on port: " << config.get("port") << std::endl;

	// setup handler function
	void (*OnReq)(evhttp_request *, void *) = [] (evhttp_request *req, void *baseDir) {
		std::string* baseDirectory = (std::string *)baseDir;
		// get output buffer
		evbuffer *OutBuf = evhttp_request_get_output_buffer(req);
		if (!OutBuf)
			return;

		std::string requestURI = evhttp_request_get_uri(req);

		std::string filename = *baseDirectory + requestURI;
		if (hasEnding(filename, "/")) {
			filename += "index.html";
		}

		double fd = open(filename.c_str(), O_RDONLY);
		if (fd >= 0) {
			struct stat st;
			stat(filename.c_str(), &st);
			evbuffer_add_file(OutBuf, fd, 0, st.st_size);
			evhttp_send_reply(req, HTTP_OK, "", OutBuf);

		} else {
			evbuffer_add_printf(OutBuf, "URI: <i>%s</i> not found", requestURI.c_str());
			evhttp_send_reply(req, HTTP_NOTFOUND, "", OutBuf);
		}
	};

	// dispatch server
	std::string baseDir = config.get("baseDir");
	evhttp_set_gencb(Server.get(), OnReq, &baseDir);
	std::cout << "Ready to serve pages." << std::endl;
	if (event_dispatch() == -1) {
		std::cerr << "Failed to run message loop." << std::endl;
		return -1;
	}

	return 0;
}
