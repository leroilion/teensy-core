/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2016 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __INTERVALTIMER_H__
#define __INTERVALTIMER_H__

#include "kinetis.h"

#ifdef __cplusplus
extern "C" {
#endif

class IntervalTimer {
#ifndef FREE_RTOS
private:
	static const uint32_t MAX_PERIOD = UINT32_MAX / (F_BUS / 1000000.0);
public:
	IntervalTimer() {
		channel = NULL;
		nvic_priority = 128;
	}
	~IntervalTimer() {
		end();
	}
	bool begin(void (*funct)(), unsigned int microseconds) {
		if (microseconds == 0 || microseconds > MAX_PERIOD) return false;
		uint32_t cycles = (F_BUS / 1000000) * microseconds - 1;
		if (cycles < 36) return false;
		return beginCycles(funct, cycles);
	}
	bool begin(void (*funct)(), int microseconds) {
		if (microseconds < 0) return false;
		return begin(funct, (unsigned int)microseconds);
	}
	bool begin(void (*funct)(), unsigned long microseconds) {
		return begin(funct, (unsigned int)microseconds);
	}
	bool begin(void (*funct)(), long microseconds) {
		return begin(funct, (int)microseconds);
	}
	bool begin(void (*funct)(), float microseconds) {
		if (microseconds <= 0 || microseconds > MAX_PERIOD) return false;
		uint32_t cycles = (float)(F_BUS / 1000000) * microseconds - 0.5;
		if (cycles < 36) return false;
		return beginCycles(funct, cycles);
	}
	bool begin(void (*funct)(), double microseconds) {
		return begin(funct, (float)microseconds);
	}
	void end();
	void priority(uint8_t n) {
		nvic_priority = n;
		#if defined(KINETISK)
		if (channel) {
			int index = channel - KINETISK_PIT_CHANNELS;
			NVIC_SET_PRIORITY(IRQ_PIT_CH0 + index, nvic_priority);
		}
		#elif defined(KINETISL)
		if (channel) {
			int index = channel - KINETISK_PIT_CHANNELS;
			nvic_priorites[index] = nvic_priority;
			if (nvic_priorites[0] <= nvic_priorites[1]) {
				NVIC_SET_PRIORITY(IRQ_PIT, nvic_priorites[0]);
			} else {
				NVIC_SET_PRIORITY(IRQ_PIT, nvic_priorites[1]);
			}
		}
		#endif
	}
	operator IRQ_NUMBER_t() {
		if (channel) {
			#if defined(KINETISK)
			int index = channel - KINETISK_PIT_CHANNELS;
			return (IRQ_NUMBER_t)(IRQ_PIT_CH0 + index);
			#elif defined(KINETISL)
			return IRQ_PIT;
			#endif
		}
		return (IRQ_NUMBER_t)NVIC_NUM_INTERRUPTS;
	}
private:
	KINETISK_PIT_CHANNEL_t *channel;
	uint8_t nvic_priority;
	#if defined(KINETISL)
	static uint8_t nvic_priorites[2];
	#endif
	bool beginCycles(void (*funct)(), uint32_t cycles);
#else // FREE_RTOS  private:
    typedef void (*ISR)();
    typedef volatile uint32_t* reg;
    enum {TIMER_OFF, TIMER_PIT};
#if defined(KINETISK)
    static const uint8_t NUM_PIT = 4;
#elif defined(KINETISL)
    static const uint8_t NUM_PIT = 2;
#endif
    static const uint32_t MAX_PERIOD = UINT32_MAX / (F_BUS / 1000000.0);
    static void enable_PIT();
    static void disable_PIT();
    static bool PIT_enabled;
    static bool PIT_used[NUM_PIT];
    static ISR PIT_ISR[NUM_PIT];
    bool allocate_PIT(uint32_t newValue);
    void start_PIT(uint32_t newValue);
    void stop_PIT();
    bool status;
    uint8_t PIT_id;
    reg PIT_LDVAL;
    reg PIT_TCTRL;
    uint8_t IRQ_PIT_CH;
    uint8_t nvic_priority;
    ISR myISR;
    bool beginCycles(ISR newISR, uint32_t cycles);
  public:
    IntervalTimer() { status = TIMER_OFF; nvic_priority = 128; }
    ~IntervalTimer() { end(); }
    bool begin(ISR newISR, unsigned int newPeriod) {
	if (newPeriod == 0 || newPeriod > MAX_PERIOD) return false;
	uint32_t newValue = (F_BUS / 1000000) * newPeriod - 1;
	return beginCycles(newISR, newValue);
    }
    bool begin(ISR newISR, int newPeriod) {
	if (newPeriod < 0) return false;
	return begin(newISR, (unsigned int)newPeriod);
    }
    bool begin(ISR newISR, unsigned long newPeriod) {
	return begin(newISR, (unsigned int)newPeriod);
    }
    bool begin(ISR newISR, long newPeriod) {
	return begin(newISR, (int)newPeriod);
    }
    bool begin(ISR newISR, float newPeriod) {
	if (newPeriod <= 0 || newPeriod > MAX_PERIOD) return false;
	uint32_t newValue = (float)(F_BUS / 1000000) * newPeriod - 0.5;
	if (newValue < 40) return false;
	return beginCycles(newISR, newValue);
    }
    bool begin(ISR newISR, double newPeriod) {
	return begin(newISR, (float)newPeriod);
    }
    void end();
    void priority(uint8_t n) {
	nvic_priority = n;
	if (PIT_enabled) NVIC_SET_PRIORITY(IRQ_PIT_CH, n);
    }
    operator IRQ_NUMBER_t() {
        if (PIT_enabled) {
#if defined(KINETISK)
            return (IRQ_NUMBER_t)(IRQ_PIT_CH + PIT_id);
#elif defined(KINETISL)
            return IRQ_PIT;
#endif
        }
        return (IRQ_NUMBER_t)NVIC_NUM_INTERRUPTS;
    }
#if defined(KINETISK)
    friend void pit0_isr();
    friend void pit1_isr();
    friend void pit2_isr();
    friend void pit3_isr();
#elif defined(KINETISL)
    friend void pit_isr();
#endif
#endif // FREE_RTOS
};


#ifdef __cplusplus
}
#endif

#endif
