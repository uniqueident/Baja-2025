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

/**
 * @brief The Timestep is a class wrapper to handle an amount of time as a float.
 */
 class Timestep {
public:
	/**
		* @brief Construct a new Timestep object with the given time.
		* 
		* @param time The amount of time the Timestep is in seconds.
		*/
	Timestep(float time = 0.0f) : m_Time(time) { }
	/**
		* @brief Construct a new Timestep object with the given Time.
		* 
		* @param time The Time to set, automatically converts to seconds.
		*/
	Timestep(Time time = 0) : m_Time(timeRealiSec(time)) { }

	/**
		* @brief Get the Seconds as a floating point value.
		* 
		* @return float 
		*/
	float GetSeconds() const { return m_Time; }
	/**
		* @brief Get the Milliseconds object as a floatting point value.
		* 
		* @return float 
		*/
	float GetMilliseconds() const { return m_Time * 1000.0f; }

	/**
		* @brief float conversion operator. 
		* 
		* @return float in seconds.
		*/
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
	float m_Time; /** @brief The amount of time, in seconds, of the Timestep. */

};

inline Timestep operator + (const Timestep& lhs, const Timestep& rhs) {
	Timestep result(lhs); result += rhs;
	return result;
}

inline Timestep operator - (const Timestep &lhs, const Timestep& rhs) {
	Timestep result(lhs); result -= rhs;
	return result;
}
