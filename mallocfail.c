#define uthash_malloc libc_malloc

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

extern int force_libc;
extern void *(*libc_malloc)(size_t);
extern void *(*libc_calloc)(size_t, size_t);
extern void *(*libc_realloc)(void *, size_t);


static char *data_file = NULL;
static char data_file_default[] = "mallocfail_counter.txt";
static int visited = 0;
static int fail_at = -1;

static int get_fail_count_to_fail_at()
{
	FILE* f = fopen(data_file, "r");
	if (f == NULL)
		return 0;
	int fails = 0;
	fscanf(f, "%d", &fails);
	fclose(f);
	return fails;
}

static int set_fail_count_to_fail_at()
{
	FILE* f = fopen(data_file, "w");
	if (f == NULL)
		return -1;
	int fails = -1;
	fprintf(f, "%d", fail_at + 1);
	fclose(f);
	return fails;
}	

int should_malloc_fail(void)
{
	force_libc = 1;
	if(!data_file){
		data_file = getenv("MALLOCFAIL_FILE");
		if(!data_file){
			data_file = data_file_default;
		}
	}

	if(fail_at == -1){
		fail_at = get_fail_count_to_fail_at();
	}
	force_libc = 0;
	
	if(visited++ == fail_at)
	{
		force_libc = 1;
		set_fail_count_to_fail_at();
		force_libc = 0;
		return 1;
	}
	
	return 0;
}

