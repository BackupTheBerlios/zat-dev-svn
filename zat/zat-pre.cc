// ZAT, ZX Assembler Toolkit.
// Copyright (c) 2004 hex@mirkforce.net
//
// $Id$
//
// The preprocessor interface.

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "zat.h"

extern char **environ;

// Makes a temporary file, forks, the child executes the
// preprocessor, the parent waits for the child to finish.
FILE* zat_pre(int, char * const * argv)
{
	int fd;
	pid_t pid;
	char fname[1024];

	strcpy(fname, "/tmp/zat-XXXXXXXX.asm");

	if ((fd = mkstemps(fname, 4)) == -1) {
		return NULL;
	}

	if (pid == 0) {
		exit(execve(BINPATH "/zapn", argv, environ));
	}

	wait(&pid);
	return NULL;
}
