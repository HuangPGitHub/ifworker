#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "appdefs.h"
#include "rpc_common.h"

int rpc_oe_client_init(cinit_data_t *init_data, char *ip)
{
    int rv = 0;

    memset(init_data, 0, sizeof(cinit_data_t));
    strcpy(init_data->broker_addr,ip);

    rv = nclt_new(init_data);
    database_init(init_data->broker_addr);

    return rv;
}

int rpc_oe_client_fini()
{
    int rv = 0;
   
    database_exit(); 
    rv = nclt_destroy();

    return rv;
}

int rpc_process_msg(string_buf *sb, unsigned long long slot_mask, int opcode, res_t **res, int *nres)
{
	int rv = 0;
	char data_buf[RPC_CLI_MAX_STR_LEN]={0};
	int size = 0;
	req_t req_data;

	if(sb->buf_size){
        //fprintf(stderr,"js:%s\n",sb->buf);
		json2msgpack(sb->buf,data_buf,&size);
        string_buf_free(sb);
	}

	nclt_req_fill(&req_data, size, opcode, slot_mask, data_buf);

	//nclt_sr(&req_data, res);
    rv = nclt_sr_plus(&req_data, res, nres, 10000);
	
	return rv;
}

int rpc_convert_val_to_json(unsigned long long value, 
                            string_buf *sb, 
                            int *in_list,
                            int *in_pair)
{
	char val[128];
	int l = 0;

	sprintf(val,"%lld",value);
	l = strlen(val);

    if ( *in_list == 0 ) {
        string_buf_puts(sb, "[", 1 );
		*in_list = 1;
    }
    else {
        if ( !(*in_pair) ) string_buf_puts(sb, ",", 1 );
    }
	
	string_buf_puts_format(sb, "%.*s", l, val);

	*in_pair=0;

	return 0;
}

int rpc_convert_spec_str_to_json(char *val,
                                string_buf *sb, 
                                int *in_list,
                                int *in_pair)
{
	int l = 0;
	l = strlen(val);
	
    if ( *in_list == 0 ) {
        string_buf_puts(sb, "[", 1 );
		*in_list = 1;
    }
    else {
        if ( !(*in_pair) ) string_buf_puts(sb, ",", 1 );
    }
	
	string_buf_puts_format(sb, "\"%.*s\"", l, val);

	*in_pair=0;
	
	return 0;
}

int rpc_convert_str_key_to_json(char *val,
                                string_buf *sb, 
                                int *in_list,
                                int *in_pair,
                                int *in_map)
{
	int l = 0;
	l = strlen(val);
	
    if ( *in_list == 0 ) {
        string_buf_puts(sb, "[", 1 );
		*in_list = 1;
    }
    else {
        if ( !(*in_pair) ) string_buf_puts(sb, ",", 1 );
    }
	
    if( *in_map == 0 ) {
		string_buf_puts(sb, "{", 1 );
        *in_map = 1;
    }

	string_buf_puts_format(sb, "%.*s:", l, val);

    *in_pair=1;
	
	return 0;
}

int rpc_convert_int_key_to_json(int val,
                                string_buf *sb, 
                                int *in_list,
                                int *in_pair,
                                int *in_map)
{
    if ( *in_list == 0 ) {
        string_buf_puts(sb, "[", 1 );
		*in_list = 1;
    }
    else {
        if ( !(*in_pair) ) string_buf_puts(sb, ",", 1 );
    }
	
    if( *in_map == 0 ) {
		string_buf_puts(sb, "{", 1 );
        *in_map = 1;
    }

	string_buf_puts_format(sb, "%d:", val);

    *in_pair=1;
	
	return 0;
}


int rpc_convert_spec_key_to_json(char *val,
                                string_buf *sb, 
                                int *in_list,
                                int *in_pair,
                                int *in_map)
{
	int l = 0;
	l = strlen(val);

    if ( *in_list == 0 ) {
        string_buf_puts(sb, "[", 1 );
		*in_list = 1;
    }
    else {
        if ( !(*in_pair) ) string_buf_puts(sb, ",", 1 );
    }

	if( *in_map == 0 ) {
		string_buf_puts(sb, "{", 1 );
        *in_map = 1;
    }
	string_buf_puts_format(sb, "\"%.*s\":", l, val);
    *in_pair=1;

	return 0;
}

int rpc_close_json(int in_list,
                int in_map,
                string_buf *sb)
{
    if ( in_map ) {
		string_buf_puts(sb, "}", 1 );
    }
    if ( in_list ) {
		string_buf_puts(sb, "]", 1 );
    }
	
	return 0;
}

