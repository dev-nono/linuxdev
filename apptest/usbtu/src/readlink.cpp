/*
 * readlink.c
 *
 *  Created on: 1 août 2018
 *      Author: bruno
 */

#define _GNU_SOURCE /* Consultez feature_test_macros(7) */
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>

int main_readlink(int argc, char *argv[])
{
	struct stat sb;
	//	char *linkname;
	char linkname[PATH_MAX];
	ssize_t r;

	char* pParam2 = 0;

    if( argc > 2 )
    {
    	pParam2=argv[2];
    }
    else
    {
    	return -1;
    }


//	if (argc != 2) {
//		fprintf(stderr, "Utilisation : %s <chemin>\n", argv[0]);
//		exit(EXIT_FAILURE);
//	}
//
	if (lstat(pParam2, &sb) == -1) {
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	printf("1 : sb.st_size=%d st_dev=0x%X  \n",
			(int)sb.st_size,
			(int)sb.st_dev
			);

	//	linkname = malloc(sb.st_size + 1);
	//	if (linkname == NULL) {
	//		fprintf(stderr, "mémoire insuffisante\n");
	//		exit(EXIT_FAILURE);
	//	}

	r = readlink(pParam2, linkname, PATH_MAX - 1);

	if (r == -1) {
		perror("readlink");
		exit(EXIT_FAILURE);
	}

	//	if (r > sb.st_size) {
	//		fprintf(stderr, "la taille du lien symbolique a augmenté"
	//				"entre lstat() et readlink()\n");
	//		exit(EXIT_FAILURE);
	//	}

	linkname[r] = '\0';

	char RelativePath[PATH_MAX];
	char AbsolutePath[PATH_MAX];
	char* pAbsolutePath = 0 ;
	char* pBasePath= 0;

	pBasePath = dirname(pParam2);

	sprintf(RelativePath,"%s/%s",pBasePath ,linkname);

	pAbsolutePath = realpath(RelativePath,AbsolutePath);
//	pAbsolutePath = canonicalize_file_name(RelativePath);

	printf("RelativePath = %s \n" , RelativePath);
	printf("path         = %s\n",		pParam2);
	printf("linkname     = %s\n",	linkname);
	printf("pPAbsolutePath = %s\n",	pAbsolutePath ? pAbsolutePath: "eeror NULL");
	printf(" errno=%d-%s\n",errno,strerror(errno));


	exit(EXIT_SUCCESS);
}
