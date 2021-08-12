#ifndef MANAGER_H
#define MANAGER_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
{
        TURNSTILE_VERSION_CMD=0x01,
        TURNSTILE_SERIAL_NUMBER_CMD=0x02,
        TURNSTILE_MOD_CMD,
        TURNSTILE_FOTOSEL_STATE_CMD,
        TURNSTILE_PASS_STATE_CMD,
        TURNSTILE_TIME_RESTRICTED_CMD,
        TURNSTILE_TIMEOUT_READ_CMD,
        TURNSTILE_FREE_PASS_REQ_CMD,
        TURNSTILE_FLAP_STATE_CMD,
        TURNSTILE_PASS_ALLOW_CMD=0x12,
        TURNSTILE_ENTER_EMERGENCY_MOD_CMD,
        TURNSTILE_ENTER_NORMAL_MOD_CMD,
        TURNSTILE_DELETE_JETON_CMD,
}turnstile_event;

       /* TURNSSTILE_SET_MODE_CMD=0x11,*/

       /* TURNSTILE_NUM_OF_MAX_CMD,*/



int send_packet(turnstile_event event);

int turnstile_set_mode(uint8_t operate_mode, uint8_t input_mode,uint8_t output_mode, uint8_t timeout, uint8_t fotosel, uint8_t serial_pass);// 0x11 CMD

int turnstile_pass_allow(uint8_t  pass_type);

int turnstile_enter_emergency_mode(void);

int turnstile_enter_normal_mode(void);

int delete_jeton(void);

int edit_timeout(uint16_t time);

int date_time_settings(uint8_t day, uint8_t mo, uint8_t year, uint8_t hour, uint8_t min, uint8_t sec);

int transparent_mode(uint8_t flap_state, uint8_t input_led, uint8_t output_led);

int send_serial_port(const char* control_data, size_t size);



const char* recived_serial_port(void);


typedef int (*send_serial_test)(const char* data, int size);
extern send_serial_test send_serial_cb;
typedef const char*(*recived_serial_test)(void);
extern recived_serial_test recived_serial_cb;

#ifdef __cplusplus
}
#endif
#endif // TURNSTILE_MANAGER_H
