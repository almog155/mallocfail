#define _GNU_SOURCE

#include "mallocfail.h"
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>


void *(*libc_malloc)(size_t) = NULL;
void *(*libc_calloc)(size_t, size_t) = NULL;
void *(*libc_realloc)(void *, size_t) = NULL;

int force_libc = 0;


static void init(void)
{

	libc_malloc = dlsym(RTLD_NEXT, "malloc");
	libc_calloc = dlsym(RTLD_NEXT, "calloc");
	libc_realloc = dlsym(RTLD_NEXT, "realloc");

	if(!libc_malloc || !libc_calloc || !libc_realloc){
		// FIXME - it would be nice to print an error here
		exit(1);
	}

}


void *malloc(size_t size)
{
	if(libc_malloc == NULL)
		init();

	if(force_libc || !should_malloc_fail()){
		return libc_malloc(size);
	}else{
		return NULL;
	}
}


void *calloc(size_t nmemb, size_t size)
{
	if(libc_calloc == NULL)
		init();

	if(force_libc || !should_malloc_fail()){
		return libc_calloc(nmemb, size);
	}else{
		return NULL;
	}
}


void *realloc(void *ptr, size_t size)
{
	if(libc_realloc == NULL) 
		init();

	if(force_libc || !should_malloc_fail()){
		return libc_realloc(ptr, size);
	}else{
		return NULL;
	}
}


