#ifndef _KERNEL_SHELL_H
#define _KERNEL_SHELL_H

#define SHELL_BUFFER_SIZE 256

void shell_init(void);

void shell_parse_input(char *input);

void shell_ls_command(void);
void shell_cd_command(char *path);
void shell_cat_command(char *path);
void shell_help_command(void);
void shell_exit_command(void);

#endif
