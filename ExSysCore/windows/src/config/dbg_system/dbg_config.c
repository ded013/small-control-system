/*
 * dbg_config.c
 *
 *  Created on: 5 сент. 2017 г.
 *      Author: Ded013
 */

#include "platform.h"
#include "config.h"

void ICACHE_FLASH_ATTR CFG_Print(DEVICE_CFG * deviceCfg)
{
	INFO("WifiPntName = %s\r\n",deviceCfg->wifi_pnt_name);
	INFO("WifiPntPassword = %s\r\n",deviceCfg->wifi_pnt_password);
	INFO("WifiPntAuthMode = %d\r\n",deviceCfg->wifi_pnt_auth_mode);
	INFO("BrokerAddress = %s\r\n",deviceCfg->mqtt_broker_address);
	INFO("BrokerPort = %d\r\n",deviceCfg->mqtt_broker_port);
	INFO("MqttClientId = %s\r\n",deviceCfg->mqtt_client_id);
	INFO("MqttUser = %s\r\n",deviceCfg->mqtt_user);
	INFO("MqttPassword = %s\r\n",deviceCfg->mqtt_password);
	INFO("MqttKeepalive = %d\r\n",deviceCfg->mqtt_keepalive);
	INFO("MqttReconnectTimeout = %d\r\n",deviceCfg->mqtt_reconnect_timeout);
	INFO("MqttDefaultSecurity = %d\r\n",deviceCfg->mqtt_default_security);
	INFO("DebugMode = %d\r\n", deviceCfg->sytem_debug_mode);
	INFO("DebugAddress = %s\r\n", deviceCfg->system_debug_address);
	INFO("DebugPort = %d\r\n", deviceCfg->system_debug_port);
	INFO("MqttDebugTopic = %s\r\n",deviceCfg->mqtt_debug_topic);
	INFO("SntpServer = %s\r\n",deviceCfg->sntp_server);
	INFO("TimeBias = %d\r\n",deviceCfg->time_bias);
	INFO("WillTopic = %s\r\n", deviceCfg->mqtt_will_topic);
	INFO("WillMessage = %s\r\n", deviceCfg->mqtt_will_message);
}
