#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "utils/utils.h"

#include "def.h"
#include "appdefs.h"
#include "rpc_common.h"

#include "mpack.h"
#include "exp_def.h"

#if 0
extern int _rpc_oe_fill_smp(OptiwayExp__Smp *pb_smp, smp_new_t *smp);
extern int _rpc_fill_counter(struct counter_t *_counter, OptiwayExp__Counter *counter);

static int _rpc_fill_smp_common(struct smp_common_t *common,  OptiwayExp__SmpCommon *smp_common)
{
    assert(common != NULL && smp_common != NULL);

    if (smp_common->has_port == 1)
    {
        common->port_id = smp_common->port;
    }
    if (smp_common->has_port_valid_flag == 1)
    {   
        common->port_valid_flag = smp_common->port_valid_flag;
    }   
    if (smp_common->has_port_insert_flag == 1) {
        common->port_insert_flag = smp_common->port_insert_flag;
    }   

    return 0;
}

static int _rpc_fill_module_common(struct sm_common_t *m_common, OptiwayExp__SmCommon *sm_common)
{
    assert(m_common != NULL && sm_common != NULL);
    
    if (sm_common->has_slot == 1)
    {   
        m_common->slot_id = sm_common->slot;
    }   
    if (sm_common->has_module == 1)
    {
        m_common->module_id = sm_common->module;
    }

    return 0;
}

int rpc_oe_set_port_autoneg(void *h, smp_new_t *smp, int onoff)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    //printf("%s line%d slot=%d,module=%d,ports=%lld:%lld:%lld:%lld,onoff=%d\n",__func__,__LINE__,
    //    smp->slots,smp->modules,smp->ports[0],smp->ports[1],smp->ports[2],smp->ports[3],onoff);
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,onoff);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/autoneg", size, buf, E_NNGX_OP_SET);
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

int rpc_oe_set_port_pause(void *h, smp_new_t *smp, int onoff)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,onoff);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/pause", size, buf, E_NNGX_OP_SET);
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

int rpc_oe_set_port_mode(smp_new_t *smp, int mode, int op)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);
    int n_res = 0, i = 0;

    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }

    rpc_convert_val_to_json(smp->slots,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->modules,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[0],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[1],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[2],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[3],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(mode,&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    slot_mask = smp->slots;
    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            rv = res[i]->rv;
            if(res[i]->content){
                free(res[i]->content); 
            }
            if(rv != 0){
                break;
            }
        }
    }
    
    return rv;
}

#if 0
int rpc_oe_set_port_shutdown(void *h, smp_new_t *smp, int shutdown)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    //printf("%s line%d slot=%d,module=%d,ports=0x%x,onoff=%d\n",__func__,__LINE__,smp->slots,smp->modules,smp->ports,onoff);
    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,shutdown);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/shut", size, buf, E_NNGX_OP_SET);
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


int rpc_oe_set_port_single_fiber_transmit(void *h, smp_new_t *smp, int able)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,able);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/single_fiber/transmit", size, buf, E_NNGX_OP_SET);
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

int rpc_oe_set_port_single_fiber_receive(void *h,smp_new_t *smp, int onoff)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,onoff);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }
    
    nngx_fill_request(&req, "/port/single_fiber/receive", size, buf, E_NNGX_OP_SET);
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


int rpc_oe_set_port_duplex(void *h, smp_new_t *smp, int duplex)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,duplex);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }
    
    nngx_fill_request(&req, "/port/duplex", size, buf, E_NNGX_OP_SET);
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

int rpc_oe_set_port_speed(void *h, smp_new_t *smp, int speed)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,speed);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/speed", size, buf, E_NNGX_OP_SET);
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

int rpc_oe_set_port_mac_address_learning(void *h, smp_new_t *smp, int onoff)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,7);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);
    mpack_write_i32(&writer,onoff);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/mac_address/learning", size, buf, E_NNGX_OP_SET);
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


int rpc_oe_set_port_mode_route(void *h, smp_new_t *smp, int onoff)
{
	int rv = 0, size = 0;
	void *buf = NULL;
	ev_nngx_request_t *req = NULL;
	ev_nngx_result_t *res = NULL;
	OptiwayExp__SmpReq smp_req = OPTIWAY_EXP__SMP_REQ__INIT;	
	OptiwayExp__Smp pb_smp = OPTIWAY_EXP__SMP__INIT;


	smp_req.smp = &pb_smp;
	_rpc_oe_fill_smp(&pb_smp, smp);

	smp_req.has_mode_route = 1;
	smp_req.mode_route = onoff;

	size = optiway_exp__smp_req__get_packed_size(&smp_req);
	buf = x_malloc(size);	
	if (buf == NULL) {
		rv = OE_ERR_NO_MEMORY;
	} else	{
		optiway_exp__smp_req__pack(&smp_req, (uint8_t *)buf);
		nngx_fill_request(&req, "/port/mode_route", size, buf, E_NNGX_OP_SET);
		rv = nngx_send_and_recv(h, req, &res);
	}
    
        if(buf)
            free(buf);

        if(req)
         free(req);

        if (res) {
            free(res);
        }

	return rv;
}

int rpc_oe_set_port_mac_ignore(void *h, smp_new_t *smp, int onoff)
{
	int rv = 0, size = 0;
	void *buf = NULL;
	ev_nngx_request_t *req = NULL;
	ev_nngx_result_t *res = NULL;
	OptiwayExp__SmpReq smp_req = OPTIWAY_EXP__SMP_REQ__INIT;	
	OptiwayExp__Smp pb_smp = OPTIWAY_EXP__SMP__INIT;


	smp_req.smp = &pb_smp;
	_rpc_oe_fill_smp(&pb_smp, smp);

	smp_req.has_mac_ignore = 1;
	smp_req.mac_ignore = onoff;

	size = optiway_exp__smp_req__get_packed_size(&smp_req);
	buf = x_malloc(size);	
	if (buf == NULL) {
		rv = OE_ERR_NO_MEMORY;
	} else	{
		optiway_exp__smp_req__pack(&smp_req, (uint8_t *)buf);
		nngx_fill_request(&req, "/port/mac_ignore", size, buf, E_NNGX_OP_SET);
		rv = nngx_send_and_recv(h, req, &res);
	}
    
        if(buf)
            free(buf);

        if(req)
    	 free(req);

        if (res) {
            free(res);
        }

	return rv;
}

int rpc_oe_set_port_default_vlan(void *h, smp_new_t *smp, int vlan_id)
{
	int rv = 0, size = 0;
	void *buf = NULL;
	ev_nngx_request_t *req = NULL;
	ev_nngx_result_t *res = NULL;
	OptiwayExp__SmpReq smp_req = OPTIWAY_EXP__SMP_REQ__INIT;	
	OptiwayExp__Smp pb_smp = OPTIWAY_EXP__SMP__INIT;


	smp_req.smp = &pb_smp;
	_rpc_oe_fill_smp(&pb_smp, smp);

	smp_req.has_default_vlan = 1;
	smp_req.default_vlan = vlan_id;

	size = optiway_exp__smp_req__get_packed_size(&smp_req);
	buf = x_malloc(size);	
	if (buf == NULL) {
		rv = OE_ERR_NO_MEMORY;
	} else	{
		optiway_exp__smp_req__pack(&smp_req, (uint8_t *)buf);
		nngx_fill_request(&req, "/port/default_vlan", size, buf, E_NNGX_OP_SET);
		rv = nngx_send_and_recv(h, req, &res);
	}
    
       if(buf)
           free(buf);
   
       if(req)
   	 free(req);
   
       if (res) {
           free(res);
       }

	return rv;
}

static int _rpc_fill_port_counter(struct port_counter_t *port_counter, OptiwayExp__SmCounter *sm_counter)
{
	int i;
	struct counter_t *counter = NULL;

	assert(port_counter != NULL && sm_counter != NULL);

	_rpc_fill_smp_common(&(port_counter->smp_common), sm_counter->smp_common);

    if (sm_counter->n_counter > 0) {
        counter = malloc(sm_counter->n_counter*sizeof(struct counter_t));
        memset(counter, 0, sm_counter->n_counter*sizeof(struct counter_t));
        port_counter->counter = counter;
        port_counter->n_counter = sm_counter->n_counter;
        for (i = 0; i < sm_counter->n_counter; i++)
            _rpc_fill_counter(&counter[i], sm_counter->counter[i]);
    }	

	return OE_STATUS_OK;
}

static int _rpc_fill_board_counter(struct board_counter_t *board_counter,  OptiwayExp__SmpListCounter *list_counter)
{
	int rv = OE_STATUS_OK;
	int i;
	OptiwayExp__SmCounter *sm_counter;

	assert(board_counter != NULL && list_counter != NULL); 

	_rpc_fill_module_common(&(board_counter->sm_common), list_counter->sm_common);
    if (list_counter->n_sm_counter > 0) {
        for(i = 0; i < list_counter->n_sm_counter; i++) {
            sm_counter = list_counter->sm_counter[i];
            rv = _rpc_fill_port_counter(&(board_counter->port_counter[i]), sm_counter);
            if (rv != 0)
                return rv;
        }
    }
	board_counter->n_port_counter = list_counter->n_sm_counter;

	return rv;
}
#endif


static int __rpc_oe_get_interface_counter(res_t *res, struct oe_counter_t *oe_counters)
{
    int rv = 0;
    mpack_reader_t reader;
    //int i = 0, j = 0, si = 0, ci = 0;
    //int slot_num = 0, cnt = 0, cnt2 = 0, cnt3 = 0;
    //struct oe_counter_t *oe_counter;
    
    mpack_reader_init(&reader, res->content, res->size, res->size);
    #if 0
    mpack_expect_array_match(&reader, 2);
    slot_num = mpack_expect_i32(&reader); /* slot number */
    mpack_expect_array_match(&reader, slot_num);
    for (si = 0; si < slot_num; si++) {
        oe_counter = &(oe_counters[si]);
        memset(oe_counter, 0, sizeof(struct oe_counter_t));
        mpack_expect_array_match(&reader, 2);
        cnt = mpack_expect_i32(&reader);
        oe_counter->n_board_counter = cnt;
        mpack_expect_array_match(&reader, cnt);
        for (i=0; i<cnt; i++) {
            mpack_expect_array_match(&reader, 4);
            oe_counter->board_counter[i].sm_common.slot_id = mpack_expect_i32(&reader);
            oe_counter->board_counter[i].sm_common.module_id = mpack_expect_i32(&reader);
            cnt2 = mpack_expect_i32(&reader);
            oe_counter->board_counter[i].n_port_counter = cnt2;
            mpack_expect_array_match(&reader, cnt2);
            for (j = 0; j < cnt2; j++) {
                mpack_expect_array_match(&reader, 5);
                oe_counter->board_counter[i].port_counter[j].smp_common.port_id = mpack_expect_i32(&reader);
                oe_counter->board_counter[i].port_counter[j].smp_common.port_valid_flag = mpack_expect_i32(&reader);
                oe_counter->board_counter[i].port_counter[j].smp_common.port_insert_flag = mpack_expect_i32(&reader);
                cnt3 = mpack_expect_i32(&reader);
                oe_counter->board_counter[i].port_counter[j].n_counter = cnt3;
                if (cnt3 > 0) {
                    oe_counter->board_counter[i].port_counter[j].counter = malloc(cnt3 * sizeof(struct counter_t));
                    if (!oe_counter->board_counter[i].port_counter[j].counter) {
                        return OE_ERR_NO_MEMORY;
                    }
                    memset(oe_counter->board_counter[i].port_counter[j].counter, 0, cnt3 * sizeof(struct counter_t));
                    mpack_expect_array_match(&reader, cnt3);
                    for (ci = 0; ci < cnt3; ci++) {
                        mpack_expect_array_match(&reader, 4);
                        oe_counter->board_counter[i].port_counter[j].counter[ci].type = mpack_expect_i32(&reader);
                        mpack_expect_str_buf(&reader,(char *)oe_counter->board_counter[i].port_counter[j].counter[ci].name,
                            sizeof(oe_counter->board_counter[i].port_counter[j].counter[ci].name));
                        oe_counter->board_counter[i].port_counter[j].counter[ci].total = mpack_expect_u64(&reader);
                        oe_counter->board_counter[i].port_counter[j].counter[ci].rate= mpack_expect_u64(&reader);
                        mpack_done_array(&reader);
                    }
                    mpack_done_array(&reader);
                }
                mpack_done_array(&reader);
            }
            mpack_done_array(&reader);
            mpack_done_array(&reader); 
        }
        mpack_done_array(&reader);
        mpack_done_array(&reader);
    }
    mpack_done_array(&reader); 
    mpack_done_array(&reader);
    #endif
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }

    return rv;
}

static int _rpc_oe_get_interface_counter(smp_new_t *smp, int verbose, struct oe_counter_t *oe_counter, int op)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);
    int n_res = 0, i = 0;

    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }

    rpc_convert_val_to_json(smp->slots,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->modules,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[0],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[1],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[2],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[3],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(verbose,&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);
    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = __rpc_oe_get_interface_counter(res[i], oe_counter);
                free(res[i]->content); 
            }
        }
    }

    return rv;
}

#if 0
int rpc_oe_get_interface_counter(void *h, smp_new_t *smp, int verbose, struct oe_counter_t *oe_counter)
{
	return _rpc_oe_get_interface_counter(h, smp, verbose, oe_counter, E_NNGX_OP_GET);
}
#endif

int rpc_oe_get_interface_counter_snmp(smp_new_t *smp, int verbose, struct oe_counter_t *oe_counter)
{
	return _rpc_oe_get_interface_counter(smp, verbose, oe_counter, OPCODE_INTERFACE_SHOW_COUNTER);
}


static int __rpc_oe_get_interface_status(res_t *res, struct oe_status_t *oe_status)
{
    int rv = 0;
    mpack_reader_t reader;
    //int i = 0, j = 0, si = 0;
    //int slot_num = 0, cnt = 0, cnt2 = 0;
    struct oe_status_t *status;
    int out_array_num = 0, inner_array_num = 0, list_num = 0;
    int head_type = 0;
    int i = 0, j = 0;
    char str[128] = {0};
    int slot = 0, module = 0;
    //mpack_tree_t tree;

    
    status = &(oe_status[0]);
#if 1
    mpack_reader_init(&reader, res->content, res->size, res->size);

    out_array_num = mpack_expect_array(&reader);
    status->n_list_status= out_array_num - 1;

    head_type = mpack_expect_u64(&reader);
    for (i=0; i<status->n_list_status; i++) {
        inner_array_num = mpack_expect_array(&reader);

        mpack_expect_map_match(&reader,1);
        mpack_expect_str_buf(&reader,str,sizeof(str));
        mpack_expect_array_match(&reader,2);
        slot = mpack_expect_i32(&reader);
        module = mpack_expect_i32(&reader);
        mpack_done_array(&reader);
        mpack_done_map(&reader);

        list_num = mpack_expect_array(&reader);
        for(j = 0; j < list_num; j++){
            mpack_expect_str_buf(&reader,str,sizeof(str));
        }
        mpack_done_array(&reader);

        status->list_status[i].n_port_status = inner_array_num - 2;
        for (j = 0; j < status->list_status[i].n_port_status; j++) {
            list_num = mpack_expect_array(&reader);

            status->list_status[i].sm_common.slot_id = mpack_expect_i32(&reader);
            status->list_status[i].sm_common.module_id = mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].smp_common.port_id = mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].smp_common.port_valid_flag = mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].smp_common.port_insert_flag = mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].link= mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].autoneg= mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].pause= mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].speed = mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].duplex= mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].mac_address_learning= mpack_expect_i32(&reader);
            status->list_status[i].port_status[j].mtu= mpack_expect_i32(&reader);
            mpack_expect_str_buf(&reader,  status->list_status[i].port_status[j].port_name, MAX_PB_STR_LEN);
            
            mpack_done_array(&reader);
        }
        mpack_done_array(&reader);
    }
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
   }

#else 
    //for debug
    mpack_tree_init(&tree,res->content,res->size);
    mpack_node_print_file(mpack_tree_root(&tree), stdout);
    mpack_tree_destroy(&tree);
#endif

    return rv;
}

static int _rpc_oe_get_interface_status_for_snmp(smp_new_t *smp, struct oe_status_t *oe_status, int op)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;
    
    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }
    
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);

    rpc_convert_val_to_json(smp->slots,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->modules,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[0],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[1],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[2],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[3],&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);
    if(rv == 0){
        for(i = 0; i < n_res; i++){
            if ((res[i]->rv == 0) && (res[i]->content)) {
                rv = __rpc_oe_get_interface_status(res[i], oe_status);
                free(res[i]->content); 
            }
        }
    }
   
    return rv;
}

#if 0
int rpc_oe_get_interface_status(void *h, smp_new_t *smp, struct oe_status_t *oe_status)
{
	return _rpc_oe_get_interface_status(smp, oe_status, OPCODE_INTERFACE_GET_STATUS_FOR_SNMP);
}
#endif

int rpc_oe_get_interface_status_snmp(smp_new_t *smp, struct oe_status_t *oe_status)
{
	return _rpc_oe_get_interface_status_for_snmp(smp, oe_status, OPCODE_INTERFACE_GET_STATUS_FOR_SNMP);
}

int rpc_oe_clear_interface_counter(smp_new_t *smp)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);

    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }
    rpc_convert_val_to_json(smp->slots,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->modules,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[0],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[1],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[2],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[3],&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    rv = rpc_process_msg(&sb,slot_mask,OPCODE_INTERFACE_CLEAR_COUNTER,res,&n_res);

    for(i = 0; i < MAX_SLOT_NUM; i++){
        if(res[i]->content){
            free(res[i]->content); 
        }
    }
    
    return rv;
}

static int _rpc_oe_get_snmp_trap_info(res_t *res, struct oe_snmp_trap_info_t *trap_info)
{
    int rv = 0;
    mpack_reader_t reader;
    int array_num = 0;
    int head_type = 0;
    char str[128] = {0};
    //mpack_tree_t tree;

    memset(trap_info, 0, sizeof(struct oe_snmp_trap_info_t));


    mpack_reader_init(&reader, res->content, res->size, res->size);
    array_num = mpack_expect_array(&reader);

    head_type = mpack_expect_i32(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->cpu_utilization.cpu_id = mpack_expect_i32(&reader); 
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->cpu_utilization.cpu_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->memory_utilization.memory_id = mpack_expect_i32(&reader); 
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->memory_utilization.memory_status = mpack_expect_i32(&reader); 
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->power.power_id = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->power.power_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->air_fan.airfan_id = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->air_fan.airfan_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->temperature.temp_id = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->temperature.temp_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    mpack_expect_str_buf(&reader,  trap_info->fcs.port_name, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->fcs.fcs_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->fcs.rate = mpack_expect_u64(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    mpack_expect_str_buf(&reader,  trap_info->drop.port_name, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->drop.drop_status = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    trap_info->drop.rate = mpack_expect_u64(&reader);
    mpack_done_map(&reader);

    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }

#if 0
    //for debug
    mpack_tree_init(&tree,res->content,res->size);
    mpack_node_print_file(mpack_tree_root(&tree), stdout);
    mpack_tree_destroy(&tree);
#endif


    return rv;
}

int rpc_oe_get_snmp_trap_info(int type, int id, struct oe_snmp_trap_info_t *trap_info)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);
    
    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }

    rpc_convert_val_to_json(type,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(id,&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    rv = rpc_process_msg(&sb,slot_mask,OPCODE_SNMP_GET_TRAP,res,&n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = _rpc_oe_get_snmp_trap_info(res[i], trap_info);
                free(res[i]->content); 
            }
        }
    }

    return rv;
}

#if 0
static int _rpc_fill_port_sfp(struct port_sfp_t *port_sfp, OptiwayExp__SmpSfp *smp_sfp)
{
	assert(port_sfp != NULL && smp_sfp != NULL);

	_rpc_fill_smp_common(&(port_sfp->smp_common), smp_sfp->smp_common);
	if (port_sfp->smp_common.port_valid_flag == 1) {
		memset(port_sfp->port_name, 0, MAX_PB_STR_LEN);
		if (smp_sfp->port_name != NULL)
			strcpy(port_sfp->port_name, smp_sfp->port_name);

		memset(port_sfp->port_descr, 0, MAX_PB_STR_LEN);
		if (smp_sfp->port_descr != NULL)
			strcpy(port_sfp->port_descr, smp_sfp->port_descr);

		if(smp_sfp->has_port_type == 1)
			port_sfp->port_type = smp_sfp->port_type;

		if(smp_sfp->has_link_proto == 1)
			port_sfp->link_proto = smp_sfp->link_proto;

		if(smp_sfp->has_running_status == 1)
			port_sfp->running_status = smp_sfp->running_status;

		if(smp_sfp->has_tx_power_flag == 1)
			port_sfp->tx_power_flag = smp_sfp->tx_power_flag;
		if(smp_sfp->has_tx_power == 1)
			port_sfp->tx_power = smp_sfp->tx_power;
		if(smp_sfp->has_tx_dbm == 1)
			port_sfp->tx_dbm = smp_sfp->tx_dbm;

		if(smp_sfp->has_rx_power_flag == 1)
			port_sfp->rx_power_flag = smp_sfp->rx_power_flag;
		if(smp_sfp->has_rx_power == 1)
			port_sfp->rx_power = smp_sfp->rx_power;
		if(smp_sfp->has_rx_dbm == 1)
			port_sfp->rx_dbm = smp_sfp->rx_dbm;

		if(smp_sfp->has_speed == 1)
			port_sfp->speed = smp_sfp->speed;

		if(smp_sfp->has_temperature_flag == 1)
			port_sfp->temperature_flag = smp_sfp->temperature_flag;
		if(smp_sfp->has_temperature == 1)
			port_sfp->temperature = smp_sfp->temperature;
	}

	return OE_STATUS_OK;
}

static int _rpc_fill_board_sfp(struct board_sfp_t *board_sfp, OptiwayExp__SmpListSfp *list_sfp)
{
	int i;

	assert(board_sfp != NULL && list_sfp != NULL);

    _rpc_fill_module_common(&(board_sfp->sm_common), list_sfp->sm_common);
    board_sfp->n_port_sfp = list_sfp->n_sfp;
    for (i = 0; i < list_sfp->n_sfp; i++) 
        _rpc_fill_port_sfp(&board_sfp->port_sfp[i], list_sfp->sfp[i]);

    return OE_STATUS_OK;
}
#endif

static int __rpc_oe_get_interface_info(res_t *res, struct oe_info_t *oe_info)
{
    int rv = 0;
#if 0    
    mpack_reader_t reader;
    int i = 0, j = 0, si = 0;
    int slot_num = 0, cnt = 0, cnt2 = 0;
    struct oe_info_t *info;

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 2);
    slot_num = mpack_expect_i32(&reader); /* slot number */
    mpack_expect_array_match(&reader, slot_num);
    for (si = 0; si < slot_num; si++) {
    info = &(oe_info[si]);
    memset(info, 0, sizeof(struct oe_info_t));
    mpack_expect_array_match(&reader, 2);
    cnt = mpack_expect_i32(&reader);
    info->n_board_sfp= cnt;
    mpack_expect_array_match(&reader, cnt);
    for (i=0; i<cnt; i++) {
    	mpack_expect_array_match(&reader, 4);
    	info->board_sfp[i].sm_common.slot_id = mpack_expect_i32(&reader);
    	 info->board_sfp[i].sm_common.module_id = mpack_expect_i32(&reader);
    	 cnt2 = mpack_expect_i32(&reader);
    	 info->board_sfp[i].n_port_sfp= cnt2;
    	mpack_expect_array_match(&reader, cnt2);
    	for (j = 0; j < cnt2; j++) {				
    		mpack_expect_array_match(&reader, 33);
    		info->board_sfp[i].port_sfp[j].smp_common.port_id = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].smp_common.port_valid_flag = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].smp_common.port_insert_flag = mpack_expect_i32(&reader);
    	       mpack_expect_str_buf(&reader, info->board_sfp[i].port_sfp[j].port_name, MAX_PB_STR_LEN);
     	       mpack_expect_str_buf(&reader, info->board_sfp[i].port_sfp[j].port_name, MAX_PB_STR_LEN);
    		info->board_sfp[i].port_sfp[j].port_type = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].link_proto = mpack_expect_i32(&reader);	
    		info->board_sfp[i].port_sfp[j].running_status = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].present = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].tx_power = mpack_expect_i64(&reader); 
    		info->board_sfp[i].port_sfp[j].tx_power_flag = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].tx_dbm = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].tx_dbm_1 = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].tx_dbm_2 = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].tx_dbm_3 = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].rx_power = mpack_expect_i64(&reader); 
    		info->board_sfp[i].port_sfp[j].rx_power_flag = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].rx_dbm = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].rx_dbm_1 = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].rx_dbm_2 = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].rx_dbm_3 = mpack_expect_i64(&reader);	
    		info->board_sfp[i].port_sfp[j].valid_window_flag = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].rx_min = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].rx_max = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].tx_min = mpack_expect_i64(&reader);
    		info->board_sfp[i].port_sfp[j].tx_max = mpack_expect_i64(&reader);
              mpack_expect_str_buf(&reader, info->board_sfp[i].port_sfp[j].vendor_name, 64);
              mpack_expect_str_buf(&reader, info->board_sfp[i].port_sfp[j].vendor_pn, 64);
    		info->board_sfp[i].port_sfp[j].wavelength = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].module_type = mpack_expect_i32(&reader); 
    		info->board_sfp[i].port_sfp[j].speed = mpack_expect_i32(&reader);
    		info->board_sfp[i].port_sfp[j].temperature = mpack_expect_i32(&reader); 
    		info->board_sfp[i].port_sfp[j].temperature_flag = mpack_expect_i32(&reader); 
    		mpack_done_array(&reader);
    	}
    	mpack_done_array(&reader);
    	mpack_done_array(&reader); 
    }
    mpack_done_array(&reader);
    mpack_done_array(&reader);
    }
    mpack_done_array(&reader); 
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
    rv = OE_ERR_PARAM_ILLEGAL;
    }
#endif
    
    return rv;
}


static int _rpc_oe_get_interface_info(smp_new_t *smp, struct oe_info_t *oe_info, int op)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;
    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);
    
    for(i = 0; i < MAX_SLOT_NUM; i++){
         memset(&res0[i], 0, sizeof(res0[i]));
         res[i] = &res0[i];
    }

    rpc_convert_val_to_json(smp->slots,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->modules,&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[0],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[1],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[2],&sb,&in_list,&in_pair);
    rpc_convert_val_to_json(smp->ports[3],&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = __rpc_oe_get_interface_info(res[i], oe_info);
                free(res[i]->content); 
            }
        }
    }
    
    return rv;
}

int rpc_oe_get_interface_info(smp_new_t *smp,  struct oe_info_t *oe_info)
{
	return _rpc_oe_get_interface_info(smp, oe_info, OPCODE_INTERFACE_GET_INFO);
}

int rpc_oe_get_interface_info_snmp(smp_new_t *smp,  struct oe_info_t *oe_info)
{
	return _rpc_oe_get_interface_info(smp, oe_info, OPCODE_INTERFACE_GET_INFO);
}

static void _rpc_free_port_counter(struct port_counter_t *port_counter)
{
    if(port_counter != NULL && port_counter->smp_common.port_valid_flag == 1) {
          if(port_counter->counter != NULL)
             free(port_counter->counter);
    }
}

static void _rpc_free_board_counter(struct board_counter_t *board_counter)
{
    int i;

    if (board_counter != NULL) {
        for(i = 0; i < board_counter->n_port_counter; i++)
            _rpc_free_port_counter(&(board_counter->port_counter[i]));
    }
}

void rpc_oe_free_interface_status( struct oe_status_t *oe_status)
{
	int i;

	if (oe_status != NULL)
	{
		if (oe_status->list_status != NULL) {
			for (i = 0; i < oe_status->n_list_status; i++)
			{
				x_free(oe_status->list_status[i].port_status);
			}
			free(oe_status->list_status);
		}
	}
}

void rpc_oe_free_interface_counter(struct oe_counter_t *oe_counter)
{
    int i;

    if (oe_counter != NULL) {
        if (oe_counter->board_counter != NULL) {
            for (i = 0; i < oe_counter->n_board_counter; i++)
                 _rpc_free_board_counter(&(oe_counter->board_counter[i]));
        }
    }
}

void rpc_oe_free_interface_info(struct oe_info_t *oe_info)
{
	//TODO
}

#if 0
static int __rpc_oe_get_interface_config(ev_nngx_result_t *res, struct oe_config_t *oe_config)
{
    int rv = 0;
    mpack_reader_t reader;
    int i = 0, j = 0, si = 0;
    int slot_num = 0, cnt = 0, cnt2 = 0;
    struct oe_config_t *config;
    
    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 2);
    slot_num = mpack_expect_i32(&reader); /* slot number */
    mpack_expect_array_match(&reader, slot_num);
    for (si = 0; si < slot_num; si++) {
        config = oe_config;
        memset(config, 0, sizeof(struct oe_config_t));
        mpack_expect_array_match(&reader, 2);
        cnt = mpack_expect_i32(&reader);
        config->n_list_config= cnt;
        mpack_expect_array_match(&reader, cnt);
        for (i=0; i<cnt; i++) {
            mpack_expect_array_match(&reader, 4);
            config->list_config[i].sm_common.slot_id = mpack_expect_i32(&reader);
            config->list_config[i].sm_common.module_id = mpack_expect_i32(&reader);
            cnt2 = mpack_expect_i32(&reader);
            config->list_config[i].n_port_config= cnt2;
            mpack_expect_array_match(&reader, cnt2);
            for (j = 0; j < cnt2; j++) {
                mpack_expect_array_match(&reader, 10);
                config->list_config[i].port_config[j].port_id = mpack_expect_i32(&reader);
                mpack_expect_str_buf(&reader,  config->list_config[i].port_config[j].port_descr, MAX_PB_STR_LEN);
                config->list_config[i].port_config[j].autoneg= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].pause= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].shutdown= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].single_fiber_transmit= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].duplex= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].speed= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].mac_address_learning= mpack_expect_i32(&reader);
                config->list_config[i].port_config[j].mode= mpack_expect_i32(&reader);
                mpack_done_array(&reader);
            }
            mpack_done_array(&reader);
            mpack_done_array(&reader); 
        }
        mpack_done_array(&reader);
        mpack_done_array(&reader);
    }
    mpack_done_array(&reader); 
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }

    return rv;
}

static int _rpc_oe_get_interface_config(void *h, smp_new_t *smp, struct oe_config_t *oe_config, int op)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,6);
    mpack_write_i32(&writer,smp->slots);
    mpack_write_i32(&writer,smp->modules);
    mpack_write_u64(&writer,smp->ports[0]);
    mpack_write_u64(&writer,smp->ports[1]);
    mpack_write_u64(&writer,smp->ports[2]);
    mpack_write_u64(&writer,smp->ports[3]);

    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/port/config/conf", size, buf, op);
    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        rv = __rpc_oe_get_interface_config(res, oe_config);
    }

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

int rpc_oe_get_interface_config(void *h, smp_new_t *smp, struct oe_config_t *oe_config)
{
	return _rpc_oe_get_interface_config(h, smp, oe_config, E_NNGX_OP_GET);
}
    
#endif
