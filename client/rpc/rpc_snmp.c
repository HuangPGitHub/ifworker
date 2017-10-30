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


#if 0
extern int _rpc_oe_fill_smp(OptiwayExp__Smp *pb_smp, smp_new_t *smp);

int rpc_oe_snmp_option(void *h, int op_snmp)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,1);
    mpack_write_i32(&writer,op_snmp);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/snmp/option", size, buf, E_NNGX_OP_SET);
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

static int _rpc_oe_show_snmp_status(ev_nngx_result_t *res, char **snmp_status)
{
    int rv = 0;
    int length;
    mpack_reader_t reader;

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 36);
    length = mpack_expect_i32(&reader);
    *snmp_status = (char *)calloc(length, sizeof(char));
    mpack_expect_str_buf(&reader,*snmp_status, length);

    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
     rv = OE_ERR_PARAM_ILLEGAL;
    }
    return rv;;
}

int rpc_oe_show_snmp_status(void *h, char **snmp_status)
{
    int rv = 0;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/snmp/status", 0, NULL, E_NNGX_OP_GET);
    rv = nngx_send_and_recv(h, req, &res);
    if(rv == 0) {
        rv = _rpc_oe_show_snmp_status(res, snmp_status);
    } else {
        rv = OE_RST_INT_ERR;
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }


    return rv;
}

static int _rpc_oe_show_snmp_conf(ev_nngx_result_t *res, struct oe_snmp_conf_t *oe_snmp_conf)
{
    int rv = 0;
    int length;
    mpack_reader_t reader;

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 36);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->access, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->auth_password, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->auth_type, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->priv_password, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->priv_type, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->rocommunity, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->rwcommunity, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->sys_contact, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->sys_location, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->sys_name, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_ip[0], length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_ip[1], length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_ip[2], length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_ip[3], length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_ip[4], length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->trap_version, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->username, length);
    length = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader,oe_snmp_conf->service, length);
    mpack_done_array(&reader);

    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }
    return rv;
}

int rpc_oe_show_snmp_conf(void *h, struct oe_snmp_conf_t *oe_snmp_conf)
{
    int rv = 0;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/snmp/config", 0, NULL, E_NNGX_OP_GET);
    rv = nngx_send_and_recv(h, req, &res);
    if(rv == 0) {
        rv = _rpc_oe_show_snmp_conf(res, oe_snmp_conf);
    } else {
        rv = OE_RST_INT_ERR;
    }

    if (req) {
        free(req);
    }

    if (res) {
        free(res);
    }

    return rv;
}

int rpc_oe_set_snmp_conf(void *h, int param, char *param_value)
{
    int rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,3);
    mpack_write_i32(&writer,param);
    mpack_write_i32(&writer,strlen(param_value) + 1);
    mpack_write_str(&writer,param_value,strlen(param_value) + 1);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/snmp/set", size, buf, E_NNGX_OP_SET);
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

static int _rpc_oe_get_snmp_entry(res_t *res, struct oe_snmp_t *oe_snmps)
{
    int rv = 0;
    mpack_reader_t reader;
    //int i = 0, j = 0, si = 0;
    //int slot_num = 0, cnt = 0, cnt2 = 0;
    struct oe_snmp_t *entry;
    int out_array_num = 0, inner_array_num = 0, list_num = 0;
    int head_type = 0;
    int i = 0, j = 0;
    char str[128] = {0};
    int slot = 0, module = 0;

    mpack_reader_init(&reader, res->content, res->size, res->size);

    entry = oe_snmps;
    out_array_num = mpack_expect_array(&reader);

    entry->n_module_snmp = out_array_num - 1;

    //head type
    head_type = mpack_expect_u64(&reader);

    for(i = 0; i < entry->n_module_snmp; i++){
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

        entry->module_snmp[i].n_port_snmp = inner_array_num - 2;
        for(j = 0; j < entry->module_snmp[i].n_port_snmp; j++){
            list_num = mpack_expect_array(&reader);
            entry->module_snmp[i].sm_common.slot_id = mpack_expect_i32(&reader);
            entry->module_snmp[i].sm_common.module_id = mpack_expect_i32(&reader);
            entry->module_snmp[i].port_snmp[j].smp_common.port_id= mpack_expect_i32(&reader);
            entry->module_snmp[i].port_snmp[j].smp_common.port_valid_flag= mpack_expect_i32(&reader);
            entry->module_snmp[i].port_snmp[j].smp_common.port_insert_flag= mpack_expect_i32(&reader);
            entry->module_snmp[i].port_snmp[j].ifInKbits = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutKbits = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInOctets = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInNUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInDiscards = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInErrors = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInUnknownProtos = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutOctets = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutNUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutDiscards = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutErrors = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutQLen = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCInOctets = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCInUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCInMulticastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCInBroadcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCOutOctets = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCOutUcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCOutMulticastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifHCOutBroadcastPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifInFcserrorPkts = mpack_expect_u64(&reader);
            entry->module_snmp[i].port_snmp[j].ifOutFcserrorPkts = mpack_expect_u64(&reader);

            mpack_done_array(&reader);
        }
        mpack_done_array(&reader);
    }

    mpack_done_array(&reader);
#if 0
    mpack_expect_array_match(&reader, 2);
    slot_num = mpack_expect_i32(&reader); /* slot number */
    mpack_expect_array_match(&reader, slot_num);
    for (si = 0; si < slot_num; si++) {
        entry = oe_snmps;
        memset(entry, 0, sizeof(struct oe_snmp_t));
        mpack_expect_array_match(&reader, 2);
        cnt = mpack_expect_i32(&reader);
        entry->n_module_snmp = cnt;
        mpack_expect_array_match(&reader, cnt);
        for (i=0; i<cnt; i++) {
            mpack_expect_array_match(&reader, 4);
            entry->module_snmp[i].sm_common.slot_id = mpack_expect_i32(&reader);
            entry->module_snmp[i].sm_common.module_id = mpack_expect_i32(&reader);
            cnt2 = mpack_expect_i32(&reader);
            entry->module_snmp[i].n_port_snmp = cnt2;
            mpack_expect_array_match(&reader, cnt2);
            for (j = 0; j < cnt2; j++) {
                mpack_expect_array_match(&reader, 27);
                entry->module_snmp[i].port_snmp[j].smp_common.port_id= mpack_expect_i32(&reader);
                entry->module_snmp[i].port_snmp[j].smp_common.port_valid_flag= mpack_expect_i32(&reader);
                entry->module_snmp[i].port_snmp[j].smp_common.port_insert_flag= mpack_expect_i32(&reader);
                entry->module_snmp[i].port_snmp[j].ifInKbits = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutKbits = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInOctets = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInNUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInDiscards = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInErrors = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInUnknownProtos = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutOctets = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutNUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutDiscards = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutErrors = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutQLen = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCInOctets = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCInUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCInMulticastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCInBroadcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCOutOctets = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCOutUcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCOutMulticastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifHCOutBroadcastPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifInFcserrorPkts = mpack_expect_u64(&reader);
                entry->module_snmp[i].port_snmp[j].ifOutFcserrorPkts = mpack_expect_u64(&reader);
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

void rpc_oe_free_snmp(struct oe_snmp_t *oe_snmp)
{
    int i;

    if (oe_snmp != NULL)
    {
        if (oe_snmp->module_snmp != NULL && oe_snmp->n_module_snmp > 0) {
            for (i = 0; i < oe_snmp->n_module_snmp; i++)
	    {
                 //x_free(oe_snmp->module_snmp[i].port_snmp);
	    }
            //free(oe_snmp->module_snmp);
        }
    }
}



int rpc_oe_get_snmp_entry(smp_new_t *smp, struct oe_snmp_t *oe_snmp)
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

    rv = rpc_process_msg(&sb,slot_mask,OPCODE_SNMP_GET_ENTRY,res,&n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = _rpc_oe_get_snmp_entry(res[i], oe_snmp);
                free(res[i]->content);
            }
        }
    }

    return rv;
}

#if 0
void rpc_oe_free_snmp_conf(struct oe_snmp_conf_t *oe_snmp_conf)
{
    int i;

    assert(oe_snmp_conf != NULL);
    x_free(oe_snmp_conf->service);
    x_free(oe_snmp_conf->sys_name);
    x_free(oe_snmp_conf->sys_contact);
    x_free(oe_snmp_conf->sys_location);
    x_free(oe_snmp_conf->rocommunity);
    x_free(oe_snmp_conf->rwcommunity);
    for (i = 0; i < MAX_SNMP_TRAP_IP_NUM; i++)
        x_free(oe_snmp_conf->trap_ip[i]);
    x_free(oe_snmp_conf->trap_version);
    x_free(oe_snmp_conf->username);
    x_free(oe_snmp_conf->auth_type);
    x_free(oe_snmp_conf->auth_password);
    x_free(oe_snmp_conf->priv_type);
    x_free(oe_snmp_conf->priv_password);
    x_free(oe_snmp_conf->access);
}
#endif
