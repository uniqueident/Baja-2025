#pragma once

#include <cstdint>


using Time = int64_t;



/**
	* @brief Gets the current time.
	* @return The current time in ticks.
	*/
Time timeNow();

/**
	* @brief Gets microseconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in microseconds.
	*/
double  timeMicroSec(Time t);
/**
	* @brief Gets milliseconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in milliseconds.
	*/
double  timeMilliSec(Time t);
/**
	* @brief Gets seconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in seconds.
	*/
double  timeRealiSec(Time t);

/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in ticks.
	*/
Time timeFrom(Time start);

/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in microseconds.
	*/
double  timeFromMicroSec(Time start);
/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in milliseconds.
	*/
double  timeFromMilliSec(Time start);
/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in seconds.
	*/
double  timeFromRealiSec(Time start);

/**
	* @brief Gets the difference in time from start to end.
	* @param start - The time to start from.
	* @param end - The time to end at.
	* @return The time in milliseconds.
	*/
double  timeDeltaMilliSec(Time start, Time end);
/**
	* @brief Gets the difference in time from start to end.
	* @param start - The time to start from.
	* @param end - The time to end at.
	* @return The time in seconds.
	*/
double  timeDeltaRealiSec(Time start, Time end);
