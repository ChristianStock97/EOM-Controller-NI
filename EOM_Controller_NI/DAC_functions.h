#pragma once
#ifndef DAC_FUNCTIONS_H_INCLUDED
#define DAC_FUNCTIONS_H_INCLUDED

#include "NIDAQmx.h"

bool create_analog_output(TaskHandle* taskHandle, short board, char channel, double range[2]);

bool create_analog_input(TaskHandle* taskHandle, short board, short channel, double range[2]);

bool writeAnalogOutput(double value);

double read_avr_value(TaskHandle* taskHandle, int sampleCount, int sampleRate);

#endif