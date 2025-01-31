#include "Time.hpp"

#if defined(_MSC_VER)

	#define WIN32_LEAN_AND_MEAN 1
	#include <Windows.h>

#else

	#include <time.h>

#endif

Time timeNow() {
#if defined(_MSC_VER)

	// Get current time
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	// Convert to microseconds
	// const i64 microseconds_per_second = 1000000LL;
	const i64 microseconds = int64_mul_div(time.QuadPart, 1000000LL, s_frequency.QuadPart);

#else

	clock_t time = clock();

	const unsigned long long microseconds = 1000000ULL * time / CLOCKS_PER_SEC;

#endif

	return microseconds;
}

double timeMicroSec(Time t) {
	return static_cast<double>(t);
}

double timeMilliSec(Time t) {
	return static_cast<double>(t) / 1000.0;
}

double timeRealiSec(Time t) {
	return static_cast<double>(t) / 1000000.0;
}

Time timeFrom(Time start) {
	return timeNow() - start;
}

double timeFromMicroSec(Time start) {
	return timeMicroSec(timeFrom(start));
}

double timeFromMilliSec(Time start) {
	return timeMilliSec(timeFrom(start));
}

double timeFromRealiSec(Time start) {
	return timeRealiSec(timeFrom(start));
}

double timeDeltaMilliSec(Time start, Time end) {
	return timeMilliSec(end - start);
}

double timeDeltaRealiSec(Time start, Time end) {
	return timeRealiSec(end - start);
}
