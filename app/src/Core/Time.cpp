#include "Time.hpp"
#include <cstdint>
#include <ctime>

#if defined(_MSC_VER)

	#define WIN32_LEAN_AND_MEAN 1
	#include <Windows.h>

#else

	#include <time.h>

#endif

Time timeNow() {
	timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);

	const uint64_t now = tp.tv_sec * 1'000'000'000 + tp.tv_nsec;
	const Time microseconds = now / 1'000;

	return microseconds;
}

double timeMicroSec(Time t) {
	return static_cast<double>(t);
}

double timeMilliSec(Time t) {
	return static_cast<double>(t) / 1'000.0;
}

double timeRealiSec(Time t) {
	return static_cast<double>(t) / 1'000'000.0;
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
