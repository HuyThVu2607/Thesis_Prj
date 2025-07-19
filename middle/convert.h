#ifndef __CONVERT_H__
#define __CONVERT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    SIGNAL_0_5V,
    SIGNAL_0_10V,
    SIGNAL_4_20mA
} SignalType;

/**
 * @brief Convert raw analog signal to engineering value
 * 
 * @param signal_type Type of input signal
 * @param input_value Voltage (V) or current (mA)
 * @param low_value User-defined value at lower bound
 * @param high_value User-defined value at upper bound
 * @return float Converted value
 */
float Convert_To_Engineering_Value(SignalType signal_type, float input_value, float low_value, float high_value);

#ifdef __cplusplus
}
#endif

#endif // __CONVERT_H__
