#include "fileservice.h"
#include "property.h"

#include <evhttp.h>
#include <iostream>
#include <memory>


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

	// start services
	FileService fileService(config.get("baseDir"), 5);
	fileService.start();
	std::cout << "FileService sarted on: " << config.get("baseDir") << std::endl;

	// setup handler function
	void (*OnReq)(evhttp_request *, void *) = [] (evhttp_request *req, void *fileService) {
		// get output buffer
		evbuffer *OutBuf = evhttp_request_get_output_buffer(req);
		if (!OutBuf)
			return;

		std::string requestURI = evhttp_request_get_uri(req);
		std::cout << "Incoming request: " << requestURI << std::endl;

		FileService* pFileService = (FileService*) fileService;
		const char* data;
		int status;
		pFileService->get(requestURI, &data, status);

		if (status == FileService::FILE_FOUND) {
			// TODO this is text, so it can just be print
			evbuffer_add_printf(OutBuf, "%s", data);
			evhttp_send_reply(req, HTTP_OK, "", OutBuf);

		} else {
			evbuffer_add_printf(OutBuf, "File: <i>%s</i> not found", requestURI.c_str());
			evhttp_send_reply(req, HTTP_OK, "", OutBuf);
		}
	};

	// dispatch server
	evhttp_set_gencb(Server.get(), OnReq, &fileService);
	std::cout << "Ready to serve pages." << std::endl;
	if (event_dispatch() == -1) {
		std::cerr << "Failed to run message loop." << std::endl;
		return -1;
	}

	return 0;
}
