/* BSD 3-Clause License
 *
 * Copyright (c) 2020, Eliot Roxbergh and Emil Hemdal for Ericsson AB
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

//#include "Enclave_u.h"

// BEGIN FROM ERLANG
#include <sys/socket.h>
#include <netinet/in.h>

#include "erl_interface.h"
#include "ei.h"
// END FROM ERLANG

#include "App.h"
#include "EnclavedErlangCFunc/Test.h"

// BEGIN FROM ERLANG
#define BUFSIZE 1000
#define SECRETCOOKIE "Very Secret Cookie"
#define CNODE_PORT 3456

int my_listen(uint16_t port);
// END FROM ERLANG


/* Application entry */
int main(int argc, char *argv[]) {
	(void) (argc);
	(void) (argv);

	// BEGIN FROM ERLANG
	uint16_t port = CNODE_PORT;
	int listen; /* Listen socket */
	int fd; /* fd to Erlang node */
	ErlConnect conn; /* Connection data */

	int loop = 1; /* Loop flag */
	int got; /* Result of receive */
	unsigned char buf[BUFSIZE]; /* Buffer for incoming message */
	ErlMessage emsg; /* Incoming message */

	ETERM *fromp, *tuplep, *fnp, *argp, *resp;
	int res;

	erl_init(NULL, 0);

	if (erl_connect_init(1, SECRETCOOKIE, 0) == -1)
		erl_err_quit("erl_connect_init");

	/* Make a listen socket */
	if ((listen = my_listen(port)) <= 0)
		erl_err_quit("my_listen");

	if (erl_publish(port) == -1)
		erl_err_quit("erl_publish");

	if ((fd = erl_accept(listen, &conn)) == ERL_ERROR)
		erl_err_quit("erl_accept");
	fprintf(stderr, "Connected to %s\n\r", conn.nodename);

	while (loop) {

		got = erl_receive_msg(fd, buf, BUFSIZE, &emsg);
		if (got == ERL_TICK) {
			/* ignore */
		} else if (got == ERL_ERROR) {
			loop = 0;
		} else {

			if (emsg.type == ERL_REG_SEND) {
				fromp = erl_element(2, emsg.msg);
				tuplep = erl_element(3, emsg.msg);
				fnp = erl_element(1, tuplep);
				argp = erl_element(2, tuplep);

				if (strncmp(ERL_ATOM_PTR(fnp), "increment", 9) == 0) {
					my_increment();
					res = 0;
				}
				else if (strncmp(ERL_ATOM_PTR(fnp), "return", 6) == 0) {
					res = my_return_a();
				}
				else{
					res=1; //TODO added this tmp
				}

				resp = erl_format("{cnode, ~i}", res);
				erl_send(fd, fromp, resp);

				erl_free_term(emsg.from);
				erl_free_term(emsg.msg);
				erl_free_term(fromp);
				erl_free_term(tuplep);
				erl_free_term(fnp);
				erl_free_term(argp);
				erl_free_term(resp);
			}
		}
	}
	// END FROM ERLANG

	return 0;
}

// BEGIN FROM ERLANG
int my_listen(uint16_t port) {
	int listen_fd;
	struct sockaddr_in addr;
	int on = 1;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (-1);

	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	memset((void*) &addr, 0, (size_t) sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		return (-1);

	listen(listen_fd, 5);
	return listen_fd;
}
// END FROM ERLANG

