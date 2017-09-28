/*
*config.c
*
*/
#include "platform.h"

#include "config.h"
#include "tag_types.h"

void ICACHE_FLASH_ATTR CFG_Load(USERCFG config_buf, DEVICE_CFG * deviceCfg)
{

	uint8_t 		_val_len;
	uint16_t 		_idx = 0;
	uint8_t 		_header_block;


	uint32_t	_size_data_buf = config_buf.cfg_len;

    _val_len = config_buf.cfg_data[_idx++];
	deviceCfg->wifi_pnt_name = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->wifi_pnt_name, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->wifi_pnt_password = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->wifi_pnt_password, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	deviceCfg->wifi_pnt_auth_mode = config_buf.cfg_data[_idx++];

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_broker_address = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_broker_address, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	os_memcpy(&deviceCfg->mqtt_broker_port, &config_buf.cfg_data[_idx], sizeof(uint16_t));
	_idx += sizeof(uint16_t);

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_client_id = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_client_id, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_user = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_user, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_password = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_password, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	os_memcpy(&deviceCfg->mqtt_keepalive, &config_buf.cfg_data[_idx], sizeof(uint16_t));
	_idx += sizeof(uint16_t);

	deviceCfg->mqtt_reconnect_timeout = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_default_security = config_buf.cfg_data[_idx++];

	 //параметры включения режима отладки
	deviceCfg->sytem_debug_mode = config_buf.cfg_data[_idx++];

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->system_debug_address = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->system_debug_address, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	os_memcpy(&deviceCfg->system_debug_port, &config_buf.cfg_data[_idx], sizeof(uint16_t));
	_idx += sizeof(uint16_t);

	//...................................

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_debug_topic = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_debug_topic, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->sntp_server = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->sntp_server, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	os_memcpy(&deviceCfg->time_bias, &config_buf.cfg_data[_idx], sizeof(short));
	_idx += sizeof(short);

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_will_topic = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_will_topic, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	_val_len = config_buf.cfg_data[_idx++];
	deviceCfg->mqtt_will_message = (char *) os_zalloc(_val_len + 1);
	os_memcpy(deviceCfg->mqtt_will_message, &config_buf.cfg_data[_idx], _val_len);
	_idx += _val_len;

	//...................................

	while (_idx < _size_data_buf)
	{
		if (_idx + sizeof(uint8_t) + sizeof(uint16_t) > _size_data_buf) return;
		_header_block = config_buf.cfg_data[_idx++];
        uint8_t _head_item = 0;
        uint8_t _size_item = 0;
        uint16_t _size_block = 0;
        uint16_t _idx_block = 0;
        uint16_t _end_block = 0;
        uint8_t _break_flag = 0;
		switch (_header_block)
		{
			case TAG_LIST_HEADER:
				os_memcpy(&_size_block, &config_buf.cfg_data[_idx], sizeof(uint16_t));
				_idx += sizeof(uint16_t);
                if ((_idx + _size_block) > _size_data_buf)
                {
                    _break_flag = 1;
                    break;
                }
                _end_block = _idx + _size_block;
                //расчет количества элементов для разных массивов адресного пространства
                _idx_block = _idx;
                while (_idx_block < _end_block)
                {
                    _head_item = config_buf.cfg_data[_idx_block++];
                    if (_head_item != TAG_ITEM_HEADER)
                    {
                        _break_flag = 1;
                        break;
                    }
                    _size_item = config_buf.cfg_data[_idx_block++];//пропуск
                    uint8_t _tag_type = config_buf.cfg_data[_idx_block];
                    increment_types(_tag_type);
                    _idx_block += _size_item;
                }
                //инициализация массивов адресного простанства для различных типов элементов
                space_initialization();

                //заполнение элементами
                while (_idx < _end_block)
                {
                    _head_item = config_buf.cfg_data[_idx++];
                    if (_head_item != TAG_ITEM_HEADER)
                    {
                        _break_flag = 1;
                        break;
                    }
                    _size_item = config_buf.cfg_data[_idx++];
                    deserialization(&config_buf.cfg_data[_idx]);
                    _idx += _size_item;
                }

				break;
			case LINK_LIST_HEADER:
				os_memcpy(&_size_block, &config_buf.cfg_data[_idx], sizeof(uint16_t));
				_idx += sizeof(uint16_t);
                if ((_idx + _size_block) > _size_data_buf)
                {
                    _break_flag = 1;
                    break;
                }
                _end_block = _idx + _size_block;
                //расчет количества соединений элементов адресного пространства
                _idx_block = _idx;
                while (_idx_block < _end_block)
                {
                    _head_item = config_buf.cfg_data[_idx_block++];
                    if (_head_item != LINK_ITEM_HEADER)
                    {
                        _break_flag = 1;
                        break;
                    }
                    _size_item = config_buf.cfg_data[_idx_block++];//пропуск
                    increment_links();
                    _idx_block += _size_item;
                }
                //инициализация массива соединений
                links_initialization();

                //заполнение соединениями и построение ссылок
                while (_idx < _end_block)
                {
                    _head_item = config_buf.cfg_data[_idx++];
                    if (_head_item != LINK_ITEM_HEADER)
                    {
                        _break_flag = 1;
                        break;
                    }
                    _size_item = config_buf.cfg_data[_idx++];
                    link_deserialization(&config_buf.cfg_data[_idx]);
                    _idx += _size_item;
                }

                //оптимизация массива соединений
                links_optimization();
				break;
			default:
				break;
		}
		if (_break_flag) break;
	}

	return;
}
