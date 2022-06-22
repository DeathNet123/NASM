#ifndef M_SHELL_STRUCT_H
#define M_SHELL_STRUCT_H

typedef struct // this struct is used as return value from the type_a function  
{
    int is_true; // will be one if the internal command exist..
    int (*fun)(char **); //will hold the pointer to the location of the function
} internal_command;

typedef struct //this struct will act as node in the linked list implementation.. 
{
    char *file_name;
    int (*fun)(char **); 
} int_cmd_node;
#endif