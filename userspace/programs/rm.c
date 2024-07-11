#include <types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	int i;

	if (argc < 2) {
		fprintf(stderr, "Usage: rm files...\n");
		exit(0);
	}

	for (i = 1; i < argc; i++) {
		if (unlink(argv[i]) < 0) {
			perror("rm failed");
			break;
		}
	}

	exit(0);
}
