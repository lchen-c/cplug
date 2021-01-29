#include "catch.hpp"
#include <vector>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <event2/thread.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <string.h>

using namespace std;

static void do_term(evutil_socket_t sig, short events, void* arg)
{
	struct event_base* base = (event_base*)arg;
	event_base_loopbreak(base);
	fprintf(stderr, "Got %i, Terminating\n", sig);
}

static int display_listen_sock(struct evhttp_bound_socket* handle)
{
	struct sockaddr_storage ss;
	evutil_socket_t fd;
	ev_socklen_t socklen = sizeof(ss);
	char addrbuf[128];
	void* inaddr;
	const char* addr;
	int got_port = -1;

	fd = evhttp_bound_socket_get_fd(handle);
	memset(&ss, 0, sizeof(ss));
	if (getsockname(fd, (struct sockaddr*)&ss, &socklen))
	{
		perror("getsockname() failed");
		return 1;
	}

	if (ss.ss_family == AF_INET)
	{
		got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
		inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
	}
	else if (ss.ss_family == AF_INET6)
	{
		got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
		inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
	}
	else
	{
		fprintf(stderr, "Weird address family %d\n",
			ss.ss_family);
		return 1;
	}

	addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
	if (addr)
	{
		printf("Listening on %s:%d\n", addr, got_port);
	}
	else
	{
		fprintf(stderr, "evutil_inet_ntop failed\n");
		return 1;
	}

	return 0;
}

void algorithm_request_cb(evhttp_request* req, void* arg)
{
	const char* cmdtype;
	struct evkeyvalq* headers;
	struct evkeyval* header;
	struct evbuffer* buf;

	switch (evhttp_request_get_command(req))
	{
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	string sUrl = evhttp_request_get_uri(req);

	std::cout << endl << endl;
	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header; header = header->next.tqe_next)
	{
		std::cout << header->key << ":" << header->value << endl;;
	}
	std::cout << endl;
	buf = evhttp_request_get_input_buffer(req);
	int  nBodyLen = evbuffer_get_length(buf);
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[1024] = { 0 };
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		std::cout << cbuf;
	}

	struct evbuffer* evbuf = evbuffer_new();
	if (!evbuf)
	{
		cout << "create evbuffer failed!" << endl;
		return;
	}

	string sResponse = "{\
	\"code\": 0,\
	\"msg\" : \"Recv Request Success!\" \
	}";
	evbuffer_add_printf(evbuf, sResponse.c_str());
	evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
	evbuffer_free(evbuf);
}


TEST_CASE("1:Test http server", "[libevent]")
{
	struct event_config* cfg = NULL;
	struct event_base* base = NULL;
	struct evhttp* http = NULL;
	struct evhttp_bound_socket* handle = NULL;
	struct evconnlistener* lev = NULL;
	struct event* term = NULL;
	int ret = 0;

#if defined (_WIN32) || defined(_WIN64)
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
#endif
	cfg = event_config_new();

#if defined (_WIN32) || defined(_WIN64)
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif
	event_config_set_num_cpus_hint(cfg, 8);
	event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);

	base = event_base_new_with_config(cfg);
	if (!base)
	{
		std::cout << "Couldn't create an event_base";
		ret = 1;
	}
	event_config_free(cfg);
	cfg = NULL;

	http = evhttp_new(base);
	if (!http)
	{
		std::cout << "Couldn't create evhttp";
		ret = 1;
	}

	evhttp_set_gencb(http, algorithm_request_cb, NULL);
	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", 8899);
	if (!handle)
	{
		std::cout << "Couldn't bind to port " << 8899 << endl;
		ret = 1;
		goto err;
	}

	if (display_listen_sock(handle))
	{
		ret = 1;
		goto err;
	}

	term = evsignal_new(base, SIGINT, do_term, base);

	if (!term)
		goto err;
	if (event_add(term, NULL))
		goto err;

	event_base_dispatch(base);

#if defined (_WIN32) || defined(_WIN64)
	WSACleanup();
#endif

err:
	if (cfg)
		event_config_free(cfg);
	if (http)
		evhttp_free(http);
	if (term)
		event_free(term);
	if (base)
		event_base_free(base);

	return;
}
