/*
 * vim: ts=8 sw=8
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define	__unused	__attribute__((unused))

static	void
print_cwd(
	char const * const	fmt,
	...
)
{
	char		buf[ PATH_MAX + 1 ];
	char		line[ PATH_MAX + 80 + 1 ];
	size_t		needed;
	va_list		ap;

	if( !getcwd( buf, sizeof( buf ) ) )	{
		perror( "getcwd" );
		exit( 1 );
	}
	va_start( ap, fmt );
	needed = vsnprintf(
		line,
		sizeof( line ),
		fmt,
		ap
	);
	va_end( ap );
	if( needed >= sizeof( line ) )	{
		puts( "Line too small for msg" );
		exit( 1 );
	}
	needed = snprintf( 
		line + needed, 
		sizeof( line ) - needed,
		"\tPid %u\t%s\n",
		getpid(),
		buf
	);
	if( needed >= sizeof( line ) )	{
		puts( "Path buffer too small" );
	} else	{
		write( fileno( stdout ), line, strlen( line ) );
	}
}

static	void *
worker(
	void *		cookie		__unused
)
{
	sleep( 5 );
	print_cwd( "Worker" );
	return( NULL );
}

int
main(
	int		argc,
	char * *	argv
)
{
	pthread_t	t;

	print_cwd( "Start" );
	if( pthread_create( &t, NULL, worker, NULL ) )	{
		perror( "pthread_create" );
		exit( 1 );
	}
	if( chdir( "/tmp" ) )	{
		perror( "chdir" );
		exit( 1 );
	}
	print_cwd( "New" );
	puts( "Sleeping..." );
	if( pthread_join( t, NULL ) )	{
		perror( "pthread_join" );
		exit( 1 );
	}
	return( 0 );
}
