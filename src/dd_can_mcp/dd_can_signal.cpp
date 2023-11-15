#include "dd_can_signal.h"
#include "dd_can_mcp.h"
#include "dd_can_matrix.h"
#include "dd_joystick/dd_joystick.h"
#include "stdint.h"
#include "dd_dypa05/dd_dypa05.h"

enum us_ID
{
  US_MOST_LEFT,
  US_LEFT,
  US_RIGHT,
  US_MOST_RIGHT,
  US_NR_OF
};

void dd_can_signal_loop(){
    dd_can_signal_dypa05();
    // dd_can_signal_joystick();
}

void dd_can_signal_dypa05(){
    uint16_t distances[US_NR_OF] = {0};

    for (int sensor_id = US_MOST_LEFT; sensor_id <= US_MOST_RIGHT; ++sensor_id)
    {
        distances[sensor_id] = dd_dypa05_get_distance(sensor_id);
    }

    uint8_t * tx_buff = dd_can_get_tx_buff_ref();

    tx_buff[0] = (distances[0] >>8) & 0xFF; // HIGH Byte X
    tx_buff[1] = distances[0] & 0xFF;       // Low Byte X

    tx_buff[2] = (distances[1] >>8) & 0xFF; // High Byte Y
    tx_buff[3] = distances[1] & 0xFF;       // Low Byte Y

    tx_buff[3] = (distances[2] >>8) & 0xFF;
    tx_buff[4] = distances[2] & 0xFF; 

    tx_buff[5] = distances[3];
}

void dd_can_signal_joystick(){
    uint16_t x = dd_joystick_get_x();
    uint16_t y = dd_joystick_get_y();
    uint8_t b = dd_joystick_get_b();

    uint8_t * tx_buff = dd_can_get_tx_buff_ref();

    tx_buff[0] = (x >>8) & 0xFF; // HIGH Byte X
    tx_buff[1] = x & 0xFF;       // Low Byte X

    tx_buff[2] = (y >>8) & 0xFF; // High Byte Y
    tx_buff[3] = y & 0xFF;       // Low Byte Y

    tx_buff[4] = b; // Button state
}

uint8_t dd_can_signal_get_bip(){
    uint32_t matrix_id = dd_can_matrix_get_msg_ref(CAN_ID_ROMAN_A);
    uint8_t * msg_buff = dd_can_matrix_get_msg_buff_ref( matrix_id);

    uint8_t bip_sig = msg_buff[4];

    return bip_sig;
}
