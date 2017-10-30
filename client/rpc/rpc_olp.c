#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>

#include "utils/utils.h"

#include "def.h"
#include "appdefs.h"
#include "rpc_common.h"

#include "mpack.h"

#if 0
int rpc_oe_olp_create(void *h, olp_id_t *id)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,2);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/create", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_destroy(void *h, olp_id_t *id)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,2);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/destroy", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_property_set(void *h, olp_id_t *id, olp_property_t *olp_property)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,6);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_str(&writer,olp_property->name,128);
    mpack_write_i32(&writer,olp_property->device);
    mpack_write_i32(&writer,olp_property->ip);
    mpack_write_i32(&writer,olp_property->port);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/property/set", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_interface_add(void *h, olp_id_t *id, smp_new_t *oe_smp)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,8);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_i32(&writer,oe_smp->slots);
    mpack_write_i32(&writer,oe_smp->modules);
    mpack_write_u64(&writer,oe_smp->ports[0]);
    mpack_write_u64(&writer,oe_smp->ports[1]);
    mpack_write_u64(&writer,oe_smp->ports[2]);
    mpack_write_u64(&writer,oe_smp->ports[3]);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/if/add", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_interface_del(void *h, olp_id_t *id, smp_new_t *oe_smp)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,8);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_i32(&writer,oe_smp->slots);
    mpack_write_i32(&writer,oe_smp->modules);
    mpack_write_u64(&writer,oe_smp->ports[0]);
    mpack_write_u64(&writer,oe_smp->ports[1]);
    mpack_write_u64(&writer,oe_smp->ports[2]);
    mpack_write_u64(&writer,oe_smp->ports[3]);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/if/del", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_interface_set_type(void *h, olp_id_t *id, smp_new_t *oe_smp, int type)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,9);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_i32(&writer,oe_smp->slots);
    mpack_write_i32(&writer,oe_smp->modules);
    mpack_write_u64(&writer,oe_smp->ports[0]);
    mpack_write_u64(&writer,oe_smp->ports[1]);
    mpack_write_u64(&writer,oe_smp->ports[2]);
    mpack_write_u64(&writer,oe_smp->ports[3]);	
    mpack_write_i32(&writer,type);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/if/set", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_work(void *h, olp_id_t *id, int on)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,3);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_i32(&writer,on);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/work", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}
#endif

static int _rpc_oe_olp_show_config(res_t *res, char **config)
{
    int rv = 0;
    int head_type=0;
    int size = 0;
    mpack_reader_t reader;
     
    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 2);
    head_type = mpack_expect_i32(&reader);
    *config = (char *)calloc(size, sizeof(char));
    if (*config == NULL) 
		return OE_ERR_NO_MEMORY;
    mpack_expect_str_buf(&reader, *config, size);	 	
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }
    
    return rv;
}

int rpc_oe_olp_show_status(olp_id_t *id, char **status)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;

    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);
   
    rpc_convert_val_to_json(id->dev_id,&sb,&in_list,&in_pair); 
    rpc_convert_val_to_json(id->link_id,&sb,&in_list,&in_pair); 
    rpc_close_json(in_list,in_map,&sb);
  
    for(i = 0; i < MAX_SLOT_NUM; i++){
        memset(&res0[i], 0, sizeof(res0[i]));
        res[i] = &res0[i];
    } 
    rv = rpc_process_msg(&sb,slot_mask,OPCODE_LKPT_LINK_STATUS_SHOW,res, &n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = _rpc_oe_olp_show_config(res[i], status);
                free(res[i]->content);
            }
        }
    }

    return rv;
}

#if 0
int rpc_oe_olp_set_value(void *h, int value, int opmode)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,1);
    mpack_write_i32(&writer,value);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    if (opmode == E_MODE_MIN_RX_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/min/rxrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_MIN_RX_FCS_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/min/rx_fcsrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_MIN_TX_FCS_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/min/tx_fcsrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_MIN_RX_TX_PERCENT) {
    	nngx_fill_request(&req, "/lkpt/link/min/percent", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_SRV_MIN_TX_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/srv/min/txrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_SRV_MIN_RX_FCS_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/srv/min/rx_fcsrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_SRV_MIN_TX_FCS_RATE) {
    	nngx_fill_request(&req, "/lkpt/link/srv/min/tx_fcsrate", size, buf, E_NNGX_OP_SET);
    } else if (opmode == E_MODE_SRV_MIN_RX_TX_PERCENT) {
    	nngx_fill_request(&req, "/lkpt/link/srv/min/percent", size, buf, E_NNGX_OP_SET);
    }

    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_interface_concatenate_type(void *h, olp_id_t *id, int type)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,3);
    mpack_write_i32(&writer,id->dev_id);
    mpack_write_i32(&writer,id->link_id);
    mpack_write_i32(&writer,type);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/if/concatenate", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
}

int rpc_oe_olp_set_heartbeat_interval(void *h, int interval)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,1);
    mpack_write_i32(&writer,interval);	
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/lkpt/link/heartbeat/interval", size, buf, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    
    if (buf) {
        free(buf);
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }
    return rv;
	
}

int rpc_oe_olp_set_heartbeat_timeout(void *h, int timeout)
{
    int rv = 0, size = 0;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__LinkReq link_req = OPTIWAY_EXP__LINK_REQ__INIT;

    link_req.has_set_value = 1;
    link_req.set_value = timeout;

    size = optiway_exp__link_req__get_packed_size(&link_req);
    buf = x_malloc(size);
    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
		optiway_exp__link_req__pack(&link_req, (uint8_t *)buf);
		nngx_fill_request(&req, "/lkpt/link/heartbeat/timeout", size, buf, E_NNGX_OP_SET);
		rv = nngx_send_and_recv(h, req, &res);
		x_free(res);
		x_free(buf);
    }

    return rv;
}
#endif
