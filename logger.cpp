#include "logger.h"
#include <iostream>

// Minimum priority needed for the log message to be outputted. Larger priority numbers infact have a lower priority, hence why it is in reverse. This increases the ease of adding new priority layers, as you can continue extended backwards.
int MinPriority = 100000000000;

void append_log_message(std::string Message, int Priority)
{
	if (Priority < MinPriority) {
		std::cout << Message << std::endl;
	}
}