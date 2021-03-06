/* 
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */
#include <stdlib.h>

#include <stdio.h>
#include <parlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <iplib.h>
#include <ndb.h>

/* concatenate two tuples */
struct ndbtuple*
ndbconcatenate(struct ndbtuple *a, struct ndbtuple *b)
{
	struct ndbtuple *t;

	if(a == NULL)
		return b;
	for(t = a; t->entry; t = t->entry)
		;
	t->entry = b;
	ndbsetmalloctag(a, getcallerpc(&a));
	return a;
}
