#ifndef __WIFI_SERVER_H_
#define __WIFI_SERVER_H_

#include "main.h"
#include "usart.h"

void client_tx_u8_data(uint8_t data_id,uint8_t data);
//void client_tx_int32_data(uint8_t data_id,int32_t data);
void client_tx_float_data(uint8_t data_id,float data);
void client_tx_int16_data(uint8_t data_id,int16_t data);
void client_tx_buf_data(uint8_t data_id, uint8_t *data_p);


#endif

