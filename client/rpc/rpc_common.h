#ifndef __RPC_COMMON_H__
#define __RPC_COMMON_H__

#include "def.h"
#include "nngn_struct.h"
#include "nngn.h"
#include "string_buf.h"

#define RPC_CLI_MAX_STR_LEN 1024
#define MAX_SLOT_NUM 14

enum {
    OE_STATUS_OK = 0,
    OE_ERR_PARAM_ILLEGAL = -1,  //not used
    OE_ERR_NO_MEMORY = -2, 
    OE_ERR_NETWORK_FALSE = -3, 
    OE_ERR_CONNECT = -4, 
    OE_ERR_UNKNOWN = -5, 
    OE_ERR_UD_DATA = -6, 
    OE_ERR_MIRROR = -7, 
} oe_error_e;

int rpc_oe_client_init(cinit_data_t *init_data, char *ip);
int rpc_oe_client_fini();
int rpc_process_msg(string_buf *sb, unsigned long long slot_mask, int opcode, res_t **res, int *nres);
int rpc_convert_val_to_json(unsigned long long, string_buf *, int *,int *);
int rpc_convert_spec_str_to_json(char *,string_buf *, int *,int *);
int rpc_convert_str_key_to_json(char *,string_buf *, int *,int *,int *);
int rpc_convert_int_key_to_json(int ,string_buf *, int *,int *,int *);
int rpc_convert_spec_key_to_json(char *,string_buf *,int *,int *,int *);
int rpc_close_json(int, int, string_buf *);
#endif
