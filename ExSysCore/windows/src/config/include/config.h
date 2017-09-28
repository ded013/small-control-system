/*
 * config.h
 *
 *  Created on: 2 сент. 2017 г.
 *      Author: Admin
 */

#ifndef CONFIG_INCLUDE_CONFIG_H_
#define CONFIG_INCLUDE_CONFIG_H_

#include "platform.h"

/* Буфер для загрузки конфигурации устройства */
#define MAX_CFG_DATA_SIZE		4088
typedef struct{
	uint32_t cfg_holder;
	uint32_t cfg_len;
	uint8_t  cfg_data[MAX_CFG_DATA_SIZE];
} USERCFG;

typedef struct{
	char * 		wifi_pnt_name;
	char *		wifi_pnt_password;
	AUTH_MODE   wifi_pnt_auth_mode;
	char *		mqtt_broker_address;
	uint16_t	mqtt_broker_port;
	char *		mqtt_client_id;
	char *		mqtt_user;
	char *		mqtt_password;
	uint16_t	mqtt_keepalive;
	uint8_t		mqtt_reconnect_timeout;
	uint8_t		mqtt_default_security; /*0 - open, 1 - SSL*/
	uint8_t		sytem_debug_mode;
	char *		system_debug_address;
	uint16_t	system_debug_port;
	char *		mqtt_debug_topic;
	char *		sntp_server;
	short       time_bias;
	char *		mqtt_will_topic;
	char * 		mqtt_will_message;
} DEVICE_CFG;

/* Заголовки элементов сохраненной конфигурации */
#define TAG_LIST_HEADER		0xF0
#define TAG_ITEM_HEADER		0xF1
#define LINK_LIST_HEADER    0xF2
#define LINK_ITEM_HEADER    0xF3


void ICACHE_FLASH_ATTR CFG_Print(DEVICE_CFG * deviceCfg);
void ICACHE_FLASH_ATTR CFG_Load(USERCFG config_buf, DEVICE_CFG * deviceCfg);
void CFG_Print(DEVICE_CFG * deviceCfg);

#endif /* CONFIG_INCLUDE_CONFIG_H_ */
