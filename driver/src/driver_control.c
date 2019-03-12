#include "driver_control.h"
#include <string.h>

static int drivers_num = 0;
static Driver_t drivers[8];

int register_dirver(const Driver_t driver)
{
	if(drivers_num < 8 -1){
		drivers[drivers_num++] = driver;
		return 0;
	}
	return 1;
}

Driver_t get_driver(const char *dev_name )
{
	int  i =0 ;
	for(i=0 ;i< drivers_num; i++){
		if(strcmp(dev_name,drivers[i]->dev_name) == 0){
			return drivers[i];
		}
	}
	return NULL;
}