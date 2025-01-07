#pragma once

#include <cstdint>
class Timestep {
public:
	Timestep(float time = 0.0f) : m_Time(time) { }

	float GetSeconds() const { return m_Time * 10.0f; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }

	operator float() const { return m_Time; }

	Timestep& operator += (const Timestep& other) {
		this->m_Time += other.m_Time;
		return *this;
	}

	Timestep& operator -= (const Timestep& other) {
		this->m_Time -= other.m_Time;
		return *this;
	}

private:
	float m_Time;

};

inline Timestep operator + (const Timestep& lhs, const Timestep& rhs) {
	Timestep result(lhs); result += rhs;
	return result;
}

inline Timestep operator - (const Timestep &lhs, const Timestep& rhs) {
	Timestep result(lhs); result -= rhs;
	return result;
}



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
