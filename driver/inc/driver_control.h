#ifndef _DRIVER_CONTROL_H__
#define _DRIVER_CONTROL_H__

#define MAX_DRIVERS 8;

typedef void*   DIRVER_HANDLE;
typedef void (*PFUNC)();
typedef DIRVER_HANDLE (*PFUNC_NAME)(DIRVER_HANDLE dev_name );
typedef int (*PFUNC_THIS)(DIRVER_HANDLE dev_handle);
typedef int  (*PFUNC_NAME_BUF_LEN)(DIRVER_HANDLE dev_handle , char *buf , int buf_len);


typedef struct Dirver{
	const char *dev_name;
	PFUNC_NAME init;
	PFUNC Open;
	PFUNC_NAME Close;
	PFUNC_NAME Flush;
	PFUNC_NAME_BUF_LEN Read;
	PFUNC_NAME_BUF_LEN Write;
	
	
}Driver ,*Driver_t;

int register_dirver(const Driver_t driver);
Driver_t get_driver(const char *name );

#endif /*DRIVER_CONTROL_H__*/

