#ifndef SDK_INPUTENUMS_HPP
#define SDK_INPUTENUMS_HPP

struct InputEvent_t
{
	int m_nType;  // Type of the event (see InputEventType_t)
	int m_nTick;  // Tick on which the event occurred
	int m_nData;  // Generic 32-bit data ,  what it contains depends on the event
	int m_nData2; // Generic 32-bit data, what it contains depends on the event
	int m_nData3; // Generic 32-bit data, what it contains depends on the event
};

#endif // SDK_INPUTENUMS_HPP
