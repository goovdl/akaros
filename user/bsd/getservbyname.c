/* 
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */
/* posix */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

/* bsd extensions */
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "priv.h"

enum
{
	Nname= 6,
};

/*
 *  for inet addresses only
 */
struct servent*
getservbyname(const char *name, const char *proto)
{
	int i, fd, m, num;
	char *p;
	char *bp;
	int nn, na;
	static struct servent s;
	static char buf[1024];
	static char *nptr[Nname+1];

	num = 1;
	for(p = (char *)name; *p; p++)
		if(!isdigit(*p))
			num = 0;

	s.s_name = 0;

	/* connect to server */
	fd = open("/net/cs", O_RDWR);
	if(fd < 0){
		return 0;
	}

	/* construct the query, always expect an ip# back */
	if(num)
		snprintf(buf, sizeof buf, "!port=%s %s=*", name, proto);
	else
		snprintf(buf, sizeof buf, "!%s=%s port=*", proto, name);

	/* query the server */
	if(write(fd, buf, strlen(buf)) < 0){
		return 0;
	}
	lseek(fd, 0, 0);
	for(i = 0; i < sizeof(buf)-1; i += m){
		m = read(fd, buf+i, sizeof(buf) - 1 - i);
		if(m <= 0)
			break;
		buf[i+m++] = ' ';
	}
	close(fd);
	buf[i] = 0;

	/* parse the reply */
	nn = na = 0;
	for(bp = buf;;){
		p = strchr(bp, '=');
		if(p == 0)
			break;
		*p++ = 0;
		if(strcmp(bp, proto) == 0){
			if(nn < Nname)
				nptr[nn++] = p;
		} else if(strcmp(bp, "port") == 0){
			s.s_port = htons(atoi(p));
		}
		while(*p && *p != ' ')
			p++;
		if(*p)
			*p++ = 0;
		bp = p;
	}
	if(nn+na == 0)
		return 0;

	nptr[nn] = 0;
	s.s_aliases = nptr;
	if(s.s_name == 0)
		s.s_name = nptr[0];

	return &s;
}
