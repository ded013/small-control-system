/*
 ============================================================================
 Name        : WinExecuteSystem.c
 Author      : 
 Version     :
 Copyright   : 
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ERROR_FILE_OPEN -3

#include "config.h"
#include "tag_types.h"

#include "espdevice_tag_types.h"
#include "mqtt_tag_types.h"
#include "rf_tag_types.h"

#ifdef ADDER_TYPE
#include "adder_tag_types.h"
#endif
#ifdef DELAYER_TYPE
#include "delayer_tag_types.h"
#endif
#ifdef SETTING_TYPE
#include "setting_tag_types.h"
#endif


int exitThreadCondition;
HANDLE theadHandle;

struct threadParams{
    int param1;
    int param2;
};

static DWORD WINAPI ThreadProc(void* threadParams)
{
    while(exitThreadCondition)
    {
    	tags_execution();
        Sleep(1);
    }

    CloseHandle(theadHandle);
    return 0;
}

static void startExecutiobnTask()
{
	exitThreadCondition = 1;
	theadHandle = CreateThread(
        NULL,                   /* default security attributes.   */
        0,                      /* use default stack size.        */
		ThreadProc,         	/* thread function name.          */
        NULL,        			/* argument to thread function.   */
        0,                      /* use default creation flags.    */
        NULL);					/* returns the thread identifier. */
}

static void stopExecutionTask()
{
	exitThreadCondition = 0;
}

static void tag_types_initialization()
{
	espdevice_type_initialization();
	mqtt_type_initialization();
	rf_sensor_type_initialization();
#ifdef ADDER_TYPE
	adder_type_initialization();
#endif
#ifdef DELAYER_TYPE
	delayer_type_initialization();
#endif
#ifdef SETTING_TYPE
	setting_type_initialization();
#endif
}

static void main_menu_help()
{
	printf("\r\nMain menu command list:\r\n\r\n");
	printf("c   - get full device config\r\n");
	printf("t   - get full tag list\r\n");
	printf("l   - get full link list\r\n");
	printf("f   - get tag full information\r\n");
	printf("v   - set value to tag\r\n");
	printf("s   - execute one cycle\r\n");
	printf("r   - run periodic execute\r\n");
	printf("p   - pause periodic execute\r\n");
    printf("lf  - set link imitation mode\r\n");
    printf("lr  - reset link imitation mode\r\n");

	printf("h   - get help info\r\n");
	printf("x   - exit programm\r\n");
}

int main(void) {
    FILE * cfg_file = NULL;
    FILE * flash_file = NULL;
	USERCFG config_buf;

    cfg_file = fopen("D:/DesktopProjects/IoTGateway/config.bin", "rb");
    if (cfg_file == NULL) {
        printf("Error opening file");
        exit(ERROR_FILE_OPEN);
    }

    fseek (cfg_file, 0, SEEK_END);
    int fsize = ftell (cfg_file);
    fseek(cfg_file, 0, SEEK_SET);
    fread(&config_buf, fsize, 1, cfg_file);
    fclose(cfg_file);

    uint16_t flash_size_block;
    void * flash_block;
    flash_file = fopen("D:/DesktopProjects/IoTGateway/flash_save.bin", "rb");
    if (flash_file != NULL)
    {
    	fseek (flash_file, 0, SEEK_END);
    	fsize = ftell (flash_file);
    	uint8_t buff[4096];
    	fseek(flash_file, 0, SEEK_SET);
    	fread(buff, fsize, 1, flash_file);
    	if (!set_saved_block(fsize, buff)) init_saved_block();
    }
    else
    	init_saved_block();
    fclose(flash_file);


    init_time_counter();
    tag_types_initialization();
    CFG_Load(&config_buf);
    execute_chain * main_chain = get_main_chain();

    char	cmd_str[5];
    memset(cmd_str, 0, sizeof(cmd_str));
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    while (1)
    {
    	printf("\r\nMain menu: choice command and ENTER, <h> to help:");
    	scanf("%s",cmd_str);
    	printf("\r\n");
    	if (strcmp(cmd_str, "h") == 0)
    	{
    		main_menu_help();
    	}
    	if (strcmp(cmd_str, "x") == 0) break;
    	if (strcmp(cmd_str, "c") == 0)
    	{
    		espdevice_config * dev_cfg = get_espdevice_config();
    		if (!dev_cfg)
    			INFO("Not find config info.\r\n");
    		else
    			CFG_Print(dev_cfg);
    	}
    	if (strcmp(cmd_str, "t") == 0)
    	{
          printf("Count tags = %d\r\n", main_chain->count_tags);
          uint16_t idx = 0;
          for (idx = 0; idx < main_chain->count_tags; idx++)
        	  dbg_head_print(main_chain->tags[idx]);
    	}
    	if (strcmp(cmd_str, "f") == 0)
    	{
    		int _id = 0;
    		printf("\r\nGet tag ID:");
    		scanf("%d",&_id);
    		generic_tag * _tag = get_tag_by_id(_id);
    		if (_tag != 0)
    			dbg_full_print(_tag);
    		else
    			printf("Not find tag with id:%d.\r\n", _id);
    	}
    	if (strcmp(cmd_str, "v") == 0)
    	{
    		int _id = 0;
    		printf("\r\nGet tag ID:");
    		scanf("%d",&_id);
    		generic_tag * _tag = get_tag_by_id(_id);
    		if (_tag != 0)
    		{
    			printf("\r\nGet input mean:");
    			scanf("%s",cmd_str);
    			if (dbg_write_input(_tag, cmd_str))
    				printf("Mean was set.\r\n");
    			else
    				printf("Mean was not set.\r\n");
    		}
    		else
    			printf("Not find tag with id:%d.\r\n", _id);
    	}
    	if (strcmp(cmd_str, "lf") == 0)
    	{
    		int _id = 0;
    		printf("\r\nGet link ID:");
    		scanf("%d",&_id);
			printf("\r\nGet input mean:");
			scanf("%s",cmd_str);
			if (link_forced_write(_id, cmd_str))
				printf("Set forced mode.\r\n");
			else
				printf("Not set forced mode.\r\n");
    	}
    	if (strcmp(cmd_str, "lr") == 0)
    	{
    		int _id = 0;
    		printf("\r\nGet link ID:");
    		scanf("%d",&_id);
			if (link_forced_reset(_id))
				printf("Reset forced mode.\r\n");
			else
				printf("Operation not complete.\r\n");
    	}
    	if (strcmp(cmd_str, "s") == 0)
    	{
    		tags_execution();
    	}
    	if (strcmp(cmd_str, "r") == 0)
    	{
    		startExecutiobnTask();
    	}
    	if (strcmp(cmd_str, "p") == 0)
    	{
    		stopExecutionTask();
    	}
    	if (strcmp(cmd_str, "l") == 0)
    	{
            printf("Count links = %d\r\n", main_chain->count_links);
            uint16_t idx = 0;
            for (idx = 0; idx < main_chain->count_links; idx++)
          	  dbg_link_print(&main_chain->links[idx]);

            printf("\r\nOrder inputs:\r\n");
            for (idx = 0; idx < main_chain->count_links; idx++)
          	  dbg_link_print(main_chain->inputs[idx]);

    	}
    }

    flash_file = fopen("D:/DesktopProjects/IoTGateway/flash_save.bin", "wb");
    if (flash_file == NULL) {
        printf("Error opening flash file");
        exit(ERROR_FILE_OPEN);
    }
    get_saved_block(&flash_size_block, &flash_block);
    fwrite(flash_block, 1, flash_size_block, flash_file);
    fclose(flash_file);
	return EXIT_SUCCESS;
}
