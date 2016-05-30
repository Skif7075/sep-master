#include "Timer.h"
#include <time.h>

void Timer::start()
{
	startSeconds = time(NULL);
}
int Timer::getCurSeconds()
{
	return time(NULL) - startSeconds;
}