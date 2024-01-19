#pragma once
#include <iostream>

// Appends a log message to the output, for efficient debugging. Default priority value is 10, if not given explicity.
void append_log_message(std::string Message, int Priority = 10);