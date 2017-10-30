#ifndef __MY_PARSER_H__
#define __MY_PARSER_H__

#include <stdio.h>
#include <stdarg.h>

#define MAX_CMD_NUM  512
#define MAX_CMD_WORD_NUM 258
#define MAX_CMD_WORD_LEN 16
#define MAX_BUF_SIZE (sizeof(char) * MAX_CMD_WORD_NUM * MAX_CMD_WORD_LEN)

extern int end_of_line;
extern char *helplist[MAX_CMD_NUM][MAX_CMD_WORD_NUM];
extern char *tokenlist[MAX_CMD_WORD_NUM][MAX_CMD_NUM];

extern int start_cli_cmd(char *cmd);
extern int start_cli_parser(char *cmd, FILE *fp);
extern char *back_token_str(char *input);
extern int find_token(char *input);
extern void init_readline(void);
extern char *command_generator(const char *text, int state);
//extern int cli_print_help(char *helplist);
extern int variable_match_check(char *var, const char *str);
extern int convert_val_to_json(unsigned long long val);
extern int convert_spec_str_to_json(char *val);
extern int convert_key_to_json(char *val);
extern int convert_spec_key_to_json(char *val);
extern int convert_spec_val_mask_to_json(char *,char *);
extern int convert_val_mask_to_json(unsigned long long, unsigned long long);
extern int convert_int_key_to_json(int);
extern int convert_range_data_to_json(int , int , int , int );
extern int convert_ud_data_to_json(int , int , int );
extern int close_json();
extern int oe_check_snmp_param(int param, char *value);
extern int get_my_slot_id(void);
extern int get_board_platform(void);
extern int is_master_om_board(void);
extern void __printf_buf(char *format, ...);
#endif
