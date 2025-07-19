#include "convert.h"

float Convert_To_Engineering_Value(SignalType signal_type, float input_value, float low_value, float high_value)
{
    float value = 0.0f;
    float range_input = 1.0f;

    switch (signal_type)
    {
        case SIGNAL_0_5V:
            if (input_value < 0.0f) return -1.0f;
            if (input_value > 5.0f) return -2.0f;
            range_input = 5.0f;
            value = low_value + (input_value / range_input) * (high_value - low_value);
            break;

        case SIGNAL_0_10V:
            if (input_value < 0.0f) return -1.0f;
            if (input_value > 10.0f) return -2.0f;
            range_input = 10.0f;
            value = low_value + (input_value / range_input) * (high_value - low_value);
            break;

        case SIGNAL_4_20mA:
            if (input_value < 4.0f) return -1.0f;
            if (input_value > 20.0f) return -2.0f;
            range_input = 16.0f;
            value = low_value + (input_value - 4.0f) * (high_value - low_value) / range_input;
            break;

        default:
            value = 0.0f;
            break;
    }

    return value;
}
