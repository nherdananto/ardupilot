
#ifndef __AP_HAL_PX4_SCHEDULER_H__
#define __AP_HAL_PX4_SCHEDULER_H__

#include <AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
#include "AP_HAL_PX4_Namespace.h"
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <systemlib/perf_counter.h>

#define PX4_SCHEDULER_MAX_TIMER_PROCS 8

#define APM_MAIN_PRIORITY    200
#define APM_TIMER_PRIORITY   201
#define APM_IO_PRIORITY       60
#define APM_OVERTIME_PRIORITY 10
#define APM_STARTUP_PRIORITY  10

/* Scheduler implementation: */
class PX4::PX4Scheduler : public AP_HAL::Scheduler {
public:
    PX4Scheduler();
    /* AP_HAL::Scheduler methods */

    void     init(void *unused);
    void     delay(uint16_t ms);
    uint32_t millis();
    uint32_t micros();
    void     delay_microseconds(uint16_t us);
    void     register_delay_callback(AP_HAL::Proc, uint16_t min_time_ms);
    void     register_timer_process(AP_HAL::TimedProc);
    void     register_timer_failsafe(AP_HAL::TimedProc, uint32_t period_us);
    void     suspend_timer_procs();
    void     resume_timer_procs();
    void     reboot();
    void     panic(const prog_char_t *errormsg);

    bool     in_timerprocess();
    bool     system_initializing();
    void     system_initialized();

private:
    bool _initialized;
    AP_HAL::Proc _delay_cb;
    uint16_t _min_delay_cb_ms;
    AP_HAL::TimedProc _failsafe;
    volatile bool _timer_pending;
    uint64_t _sketch_start_time;

    volatile bool _timer_suspended;
    AP_HAL::TimedProc _timer_proc[PX4_SCHEDULER_MAX_TIMER_PROCS];
    uint8_t _num_timer_procs;
    volatile bool _in_timer_proc;
    volatile bool _timer_event_missed;

    pthread_t _timer_thread_ctx;
    pthread_t _io_thread_ctx;

    void *_timer_thread(void);
    void *_io_thread(void);

    void _run_timers(bool called_from_timer_thread);

    perf_counter_t  _perf_timers;
    perf_counter_t  _perf_delay;
};
#endif
#endif // __AP_HAL_PX4_SCHEDULER_H__


