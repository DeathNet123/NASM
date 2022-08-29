#ifndef CONSTANT_H
#define CONSTANT_H
#include<linux/limits.h>

#define MAX_CMD_LEN 4096 // The max lenght of the command at one time. this is same as bash.....
#define CONTROL_COMMAND_GROUP 4 //Groups count for the control command regex
#define ARG_NUM 50 //number of arguments that can be passed to the command either external or internal..
#define ARG_LEN_LS PATH_MAX //lenght of each argument the PATH_MAX is choosen to satisfy the condition for the ls command..
#define SABNORMAL -60 //this the main exit status...
#define COMMAND_NOT_FOUND 32512
#define COM_NOT_EXIST 127
#define PIPE_CMD_GROUP 3
#define FILE_NOT_FOUND -56565
#endif