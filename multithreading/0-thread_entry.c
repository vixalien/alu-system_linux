#include "multithreading.h"

/**
 * thread_entry - serves as entry point to new thread
 * @arg: holds address of string that must be printed and followed by new line
 * Return: void
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
}
