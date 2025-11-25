#include "multithreading.h"

void mutex_init(void) __attribute__((constructor));
void mutex_destroy(void) __attribute__((destructor));
static pthread_mutex_t lock_01;

/**
 * mutex_init - initializes a mutex lock
 */
void mutex_init(void)
{
	if (pthread_mutex_init(&lock_01, NULL) != 0)
	{
		printf("Mutex init failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * mutex_destroy - destroys a mutex lock
 */
void mutex_destroy(void)
{
	if (pthread_mutex_destroy(&lock_01) != 0)
	{
		printf("Mutex destroy failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * tprintf - uses printf family to print out given formatted string
 * @format: string to print
 * Return: EXIT_SUCCESS
 */
int tprintf(char const *format, ...)
{
	va_list arguments;

	va_start(arguments, format);

	/* lock mutex */
	if (pthread_mutex_lock(&lock_01) != 0)
	{
		printf("Mutex Lock Failed\n");
		exit(EXIT_FAILURE);
	}

	/* print calling thread ID, and string */
	printf("[%lu] ", pthread_self());
	vprintf(format, arguments);

	/* unlock mutex */
	pthread_mutex_unlock(&lock_01);

	va_end(arguments);
	return (EXIT_SUCCESS);
}
