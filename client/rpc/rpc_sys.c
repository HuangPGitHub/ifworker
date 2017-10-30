#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "utils/utils.h"

#include "appdefs.h"
#include "rpc_common.h"

#include "mpack.h"
#include "srv_mpack_common.h"

#if 0
static int __rpc_oe_get_config(void *h, ev_nngx_result_t *res, char **config,int *page_total)
{
    int rv = 0;
    mpack_reader_t reader;
    int length = 0;

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 3);
    length = mpack_expect_i32(&reader);
    *config = (char *)calloc(length, sizeof(char));
    mpack_expect_str_buf(&reader, *config, length);
    (*config)[length] = '\0';
    *page_total = mpack_expect_i32(&reader);
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }
    return rv;
}

int rpc_oe_get_config(void *h, int page_size,int page_index,char **config,int *page_total)
{
    int rv = 0, temp_rv = 0;
    size_t size;
    char *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    mpack_writer_t writer;

    mpack_writer_init_growable(&writer, &buf, &size);
    mpack_start_array(&writer,2);
    mpack_write_i32(&writer,page_size);
    mpack_write_i32(&writer,page_index);
    mpack_finish_array(&writer);
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        free(buf);
        return OE_ERR_PARAM_ILLEGAL;
    }

    nngx_fill_request(&req, "/config/conf", size, buf, E_NNGX_OP_GET);
    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        temp_rv = __rpc_oe_get_config(h, res, config,page_total);
        if (temp_rv != 0) {
            rv = temp_rv;
        }
    }

    if(res != NULL)
        x_free(res);

    return rv;
}

static int _rpc_fill_system_soft(struct oe_system_t *system, OptiwayExp__SysSoft *sys_soft)
{
    int i;

    if (sys_soft->device_name == NULL || sys_soft->device_type == NULL
            || sys_soft->software_version == NULL || sys_soft->sys_time == NULL
            || sys_soft->sys_uptime == NULL || sys_soft->sys_uptime->has_day == 0
            || sys_soft->sys_uptime->has_hour== 0 || sys_soft->sys_uptime->has_minute == 0
            || sys_soft->sys_uptime->has_sec == 0 || sys_soft->cpu_utilization == NULL
            || sys_soft->memory_utilization == NULL || sys_soft->power == NULL
            || sys_soft->air_fan == NULL || sys_soft->build_date == NULL
            || sys_soft->build_time == NULL || sys_soft->system_ip == NULL
            || sys_soft->system_gw == NULL || sys_soft->has_platform == 0)
        return OE_ERR_NETWORK_FALSE;

    strcpy(system->device_name, sys_soft->device_name);
    strcpy(system->device_type, sys_soft->device_type);
    strcpy(system->software_version, sys_soft->software_version);
    strcpy(system->sys_time, sys_soft->sys_time);

    system->sys_uptime.up_day = sys_soft->sys_uptime->day;
    system->sys_uptime.up_hour = sys_soft->sys_uptime->hour;
    system->sys_uptime.up_minute = sys_soft->sys_uptime->minute;
    system->sys_uptime.up_sec = sys_soft->sys_uptime->sec;

    system->cpu_utilization = (struct cpu_utilization_t **)calloc(sys_soft->n_cpu_utilization, sizeof(struct cpu_utilization_t));
    if (system->cpu_utilization == NULL) {
        return OE_ERR_NO_MEMORY;
    }
    system->n_cpu = sys_soft->n_cpu_utilization;
    for (i = 0; i < sys_soft->n_cpu_utilization; i++) {
        if (sys_soft->cpu_utilization[i] == NULL) {
            return OE_ERR_NETWORK_FALSE;
        }
        system->cpu_utilization[i] = (struct cpu_utilization_t *)malloc(sizeof(struct cpu_utilization_t));
        system->cpu_utilization[i]->cpu_util_5_30s = sys_soft->cpu_utilization[i]->cpu_util_5_30s;
        system->cpu_utilization[i]->cpu_util_1_5m = sys_soft->cpu_utilization[i]->cpu_util_1_5m;
    }

    system->memory_utilization = (struct memory_utilization_t **)calloc(sys_soft->n_memory_utilization, sizeof(struct memory_utilization_t));
    if (system->memory_utilization == NULL) {
        return OE_ERR_NO_MEMORY;
    }
    system->n_memory = sys_soft->n_memory_utilization;
    for (i = 0; i < sys_soft->n_memory_utilization; i++) {
        if (sys_soft->memory_utilization[i] == NULL) {
            return OE_ERR_NETWORK_FALSE;
        }
        system->memory_utilization[i] = (struct memory_utilization_t *)malloc(sizeof(struct memory_utilization_t));
        system->memory_utilization[i]->max_memory = sys_soft->memory_utilization[i]->max_memory;
        system->memory_utilization[i]->memory_utilization = sys_soft->memory_utilization[i]->memory_utilization;
    }

    system->power = (struct power_t **)calloc(sys_soft->n_power, sizeof(struct power_t));
    if (system->power == NULL) {
        return OE_ERR_NO_MEMORY;
    }
    system->n_power = sys_soft->n_power;
    for (i = 0; i < sys_soft->n_power; i++) {
        if (sys_soft->power[i] == NULL) {
            return OE_ERR_NETWORK_FALSE;
        }
        system->power[i] = (struct power_t *)malloc(sizeof(struct power_t));
        system->power[i]->power_id = sys_soft->power[i]->power_id;
        system->power[i]->power_status = sys_soft->power[i]->power_status;
    }

    system->air_fan = (struct air_fan_t **)calloc(sys_soft->n_air_fan, sizeof(struct air_fan_t));
    if (system->air_fan == NULL) {
        return OE_ERR_NO_MEMORY;
    }
    system->n_air_fan = sys_soft->n_air_fan;
    for (i = 0; i < sys_soft->n_air_fan; i++) {
        if (sys_soft->air_fan[i] == NULL) {
            return OE_ERR_NETWORK_FALSE;
        }
        system->air_fan[i] = (struct air_fan_t *)malloc(sizeof(struct air_fan_t));
        system->air_fan[i]->airfan_id = sys_soft->air_fan[i]->airfan_id;
        system->air_fan[i]->airfan_status = sys_soft->air_fan[i]->airfan_status;
    }

    strcpy(system->build_date, sys_soft->build_date);
    strcpy(system->build_time, sys_soft->build_time);
    strcpy(system->system_ip, sys_soft->system_ip);
    strcpy(system->system_gateway, sys_soft->system_gw);
    system->platform = sys_soft->platform;

    return 0;
}
#endif

int rpc_oe_free_system(struct oe_system_t *system)
{
    assert(system != NULL);

    return 0;
}

static int __rpc_oe_get_system(res_t *res, struct oe_system_t *system)
{
    int rv = 0;
    mpack_reader_t reader;
    int si = 0;
    int array_num = 0, c_array_num = 0;
    int head_type = 0;
    char str[128] = {0};
    //mpack_tree_t tree;

#if 1
    mpack_reader_init(&reader, res->content, res->size, res->size);

    array_num = mpack_expect_array(&reader);
    head_type = mpack_expect_i32(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->software_version, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->systemname, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->sys_time, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_array_match(&reader,4);
    system->sys_uptime.up_day = mpack_expect_i32(&reader);
    system->sys_uptime.up_hour = mpack_expect_i32(&reader);
    system->sys_uptime.up_minute = mpack_expect_i32(&reader);
    system->sys_uptime.up_sec = mpack_expect_i32(&reader);
    mpack_done_array(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    c_array_num = mpack_expect_array(&reader);
    system->n_cpu = c_array_num;

    for (si = 0; si <  system->n_cpu  ; si++) {
        mpack_expect_array_match(&reader,2);
        system->cpu_utilization[si].cpu_util_5_30s = mpack_expect_i32(&reader);
        system->cpu_utilization[si].cpu_util_1_5m= mpack_expect_i32(&reader);
        mpack_done_array(&reader);
    }
    mpack_done_array(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    c_array_num = mpack_expect_array(&reader);
    system->n_memory = c_array_num;

    for (si = 0; si <  system->n_memory  ; si++) {
         mpack_expect_array_match(&reader, 2);
         system->memory_utilization[si].max_memory = mpack_expect_u32(&reader);
         system->memory_utilization[si].memory_utilization= mpack_expect_i32(&reader);
         mpack_done_array(&reader);
     }
    mpack_done_array(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    c_array_num = mpack_expect_array(&reader);
    system->n_power = c_array_num;

    for (si = 0; si <  system->n_power  ; si++) {
       mpack_expect_array_match(&reader, 2);
       system->power[si].power_id = mpack_expect_i32(&reader);
       system->power[si].power_status= mpack_expect_i32(&reader);
       mpack_done_array(&reader);
    }

    mpack_done_array(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    c_array_num = mpack_expect_array(&reader);
    system->n_air_fan = c_array_num;

    for (si = 0; si <  system->n_air_fan  ; si++) {
       mpack_expect_array_match(&reader, 2);
       system->air_fan[si].airfan_id = mpack_expect_i32(&reader);
       system->air_fan[si].airfan_status= mpack_expect_i32(&reader);
       mpack_done_array(&reader);
    }

    mpack_done_array(&reader);
    mpack_done_map(&reader);

    if(array_num == 18){
        mpack_expect_map_match(&reader,1);
        mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
        c_array_num = mpack_expect_array(&reader);
        for(si=0;si<c_array_num;si++){
	        system->temps[si] = mpack_expect_i32(&reader);
        }
        mpack_done_array(&reader);
        mpack_done_map(&reader);
    }

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    system->fan1_def_level = mpack_expect_u8(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    system->fan1_cur_level = mpack_expect_u8(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    system->fan2_def_level = mpack_expect_u8(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    system->fan2_cur_level = mpack_expect_u8(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_array_match(&reader,2);
    mpack_expect_str_buf(&reader, system->build_date, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->build_time, MAX_PB_STR_LEN);
    mpack_done_array(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->system_ip, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, system->system_gateway, MAX_PB_STR_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader,1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    system->platform = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

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

static int _rpc_oe_get_system(struct oe_system_t *system, int op)
{
    int rv = 0, temp_rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int n_res = 0, i = 0;

    memset(&sb, 0, sizeof(sb));
    for(i = 0; i < MAX_SLOT_NUM; i++){
        memset(&res0[i], 0, sizeof(res0[i]));
        res[i] = &res0[i];
    }

    rv = rpc_process_msg(&sb,slot_mask,OPCODE_SYSTEM_GET_SYSTEM,res,&n_res);
    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                temp_rv = __rpc_oe_get_system(res[i], system);
                if (temp_rv != 0) {
                    rv = temp_rv;
                }
                free(res[i]->content);
            }
        }

    }

    return rv;
}

int rpc_oe_get_system(struct oe_system_t *system)
{
    return _rpc_oe_get_system(system, OPCODE_SYSTEM_GET_SYSTEM);
}

int rpc_oe_get_system_snmp(struct oe_system_t *system)
{
    return _rpc_oe_get_system(system, OPCODE_SYSTEM_GET_SYSTEM);
}

static int __rpc_oe_get_board_info(res_t *res, struct board_info_t *board_info)
{
    int rv = 0;
    mpack_reader_t reader;
    int array_num = 0;
    int head_type = 0;
    char str[MAX_PB_STR_LEN] = {0};

    mpack_reader_init(&reader, res->content, res->size, res->size);

    #if 0
    mpack_expect_array_match(&reader, 8);
    board_info->type = mpack_expect_i32(&reader);
    mpack_expect_str_buf(&reader, board_info->version, MAX_STRING_LEN);
    mpack_expect_str_buf(&reader, board_info->serial_num, MAX_STRING_LEN);
    board_info->slot_id = mpack_expect_i32(&reader);
    board_info->status= mpack_expect_i32(&reader);
    board_info->om_switchover= mpack_expect_i32(&reader);
    board_info->temprature= mpack_expect_i32(&reader);
    board_info->up_time= mpack_expect_i32(&reader);
    mpack_done_array(&reader);
    #endif
    array_num = mpack_expect_array(&reader);
    head_type = mpack_expect_i32(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->type = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, board_info->version, MAX_STRING_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    mpack_expect_str_buf(&reader, board_info->serial_num, MAX_STRING_LEN);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->slot_id = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->status= mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->om_switchover= mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->temprature= mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader, str, MAX_PB_STR_LEN);
    board_info->up_time= mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }
    return rv;
}

static int _rpc_oe_get_board_info(int slot_id, struct board_info_t *board_info, int op)
{
    int rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int in_list = 0, in_pair = 0, in_map = 0;
    int n_res = 0, i = 0;

    string_buf_init(&sb,RPC_CLI_MAX_STR_LEN);

    rpc_convert_val_to_json(slot_id,&sb,&in_list,&in_pair);
    rpc_close_json(in_list,in_map,&sb);

    slot_mask |= 1 << (slot_id - 1);

    for(i = 0; i < MAX_SLOT_NUM; i++){
        memset(&res0[i], 0, sizeof(res0[i])); 
        res[i] = &res0[i]; 
    }

    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);

    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if((res[i]->rv == 0) && (res[i]->content)){
                rv = __rpc_oe_get_board_info(res[i], board_info);
                free(res[i]->content);
            }
        }
    }

    return rv;
}

int rpc_oe_get_board_info(int slot_id, struct board_info_t *board_info)
{
    return _rpc_oe_get_board_info(slot_id, board_info, OPCODE_SYSTEM_GET_BOARDINFO);
}

int rpc_oe_get_board_info_snmp(int slot_id, struct board_info_t *board_info)
{
    return _rpc_oe_get_board_info(slot_id, board_info, OPCODE_SYSTEM_GET_BOARDINFO);
}

int rpc_oe_save_or_get_config(int op, res_t **res, int *nres)
{
    int rv = 0, i = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    int n_res = 0;

    memset(&sb, 0, sizeof(string_buf));
    rv = rpc_process_msg(&sb,slot_mask,op,res,&n_res);
    if(rv == 0){
        *nres = n_res;
    }

    return rv;
}


#if 0
static int __rpc_oe_get_board_status(void *h,
        ev_nngx_result_t *res,
        int status[SLOT_NUMBER_PER_SYSTEM],
        int type[SLOT_NUMBER_PER_SYSTEM])
{
    int rv = 0;
    mpack_reader_t reader;
    int i = 0;
    int slot_num = 0;

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 2);
    slot_num = mpack_expect_i32(&reader); /* slot number */
    printf("\r\n %s %d slot_num=%d 1\n\r",__func__,__LINE__,slot_num);
    mpack_expect_array_match(&reader, slot_num);

    for (i = 0; i < slot_num; i++) {
        mpack_expect_array_match(&reader, 2);
        status[i] = mpack_expect_i32(&reader);
        type[i] =  mpack_expect_i32(&reader);
	 printf("\r\n %s %d status[%d]=%d  type[%d]=%d \n\r",__func__,__LINE__,i,status[i],i,type[i]);
	 mpack_done_array(&reader);
    }

    mpack_done_array(&reader);
    mpack_done_array(&reader);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }

    return rv;
}

static int _rpc_oe_get_board_status(void *h,
        int status[SLOT_NUMBER_PER_SYSTEM],
        int type[SLOT_NUMBER_PER_SYSTEM],
        int op)
{
    int rv = 0, temp_rv = 0;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/board/status", 0, NULL, op);
    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        temp_rv = __rpc_oe_get_board_status(h, res, status, type);
        if (temp_rv != 0) {
            rv = temp_rv;
        }
    }
    x_free(res);

    return rv;
}

int rpc_oe_get_board_status(void *h,
        int status[SLOT_NUMBER_PER_SYSTEM],
        int type[SLOT_NUMBER_PER_SYSTEM])
{
    return _rpc_oe_get_board_status(h, status ,type, E_NNGX_OP_GET);
}

int rpc_oe_get_board_status_snmp(void *h,
        int status[SLOT_NUMBER_PER_SYSTEM],
        int type[SLOT_NUMBER_PER_SYSTEM])
{
    return _rpc_oe_get_board_status(h, status ,type, E_NNGX_OP_SNMP);
}

int rpc_oe_reset_system(void *h)
{
    int rv = 0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/system/reset", 0, NULL, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    x_free(res);

    return rv;
}

int rpc_oe_restore_config(void *h)
{
    int rv = 0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/config/restore", 0, NULL, E_NNGX_OP_SET);
    rv = nngx_send_and_recv(h, req, &res);
    x_free(res);

    return rv;
}


int rpc_oe_set_platform(void *h, int slot, int platform)
{
    int rv = 0, size = 0;
    OptiwayExp__SysSoft sys_soft = OPTIWAY_EXP__SYS_SOFT__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    sys_soft.has_slot_id = 1;
    sys_soft.slot_id = slot;
    sys_soft.has_platform = 1;
    sys_soft.platform = platform;

    size = optiway_exp__sys_soft__get_packed_size(&sys_soft);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_soft__pack(&sys_soft, (uint8_t *)buf);
        nngx_fill_request(&req, "/platform", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_import_config(void *h, int ip, char *username, char *password , char *filename, char *filepath)
{
    int rv = 0, size = 0;
    OptiwayExp__ConfigManage  config_manage = OPTIWAY_EXP__CONFIG_MANAGE__INIT;
    char tmp_username[256] = {0};
    char tmp_password[256] = {0};
    char tmp_filename[256] = {0};
    char tmp_filepath[256] = {0};
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    strcpy(tmp_username, username);
    tmp_username[strlen(username)] = 0;
    strcpy(tmp_password, password);
    tmp_password[strlen(password)] = 0;
    strcpy(tmp_filename, filename);
    tmp_filename[strlen(filename)] = 0;
    strcpy(tmp_filepath, filepath);
    tmp_filepath[strlen(filepath)] = 0;

    config_manage.has_ip_addr = 1;
    config_manage.ip_addr = ip;
    config_manage.username = tmp_username;
    config_manage.password = tmp_password;
    config_manage.filename = tmp_filename;
    config_manage.filepath = tmp_filepath;

    size = optiway_exp__config_manage__get_packed_size(&config_manage);
    buf = x_malloc(size);

    if (buf == NULL)
        rv = OE_ERR_NO_MEMORY;
    else {
        optiway_exp__config_manage__pack(&config_manage, (uint8_t *)buf);
        nngx_fill_request(&req, "/config/import", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }

    return rv;
}

int rpc_oe_export_config(void *h, int ip, char *username, char *password , char *filename, char *filepath)
{
    int rv = 0, size = 0;
    OptiwayExp__ConfigManage  config_manage = OPTIWAY_EXP__CONFIG_MANAGE__INIT;
    char tmp_username[256] = {0};
    char tmp_password[256] = {0};
    char tmp_filename[256] = {0};
    char tmp_filepath[256] = {0};
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    strcpy(tmp_username, username);
    tmp_username[strlen(username)] = 0;
    strcpy(tmp_password, password);
    tmp_password[strlen(password)] = 0;
    strcpy(tmp_filename, filename);
    tmp_filename[strlen(filename)] = 0;
    strcpy(tmp_filepath, filepath);
    tmp_filepath[strlen(filepath)] = 0;

    config_manage.has_ip_addr = 1;
    config_manage.ip_addr = ip;
    config_manage.username = tmp_username;
    config_manage.password = tmp_password;
    config_manage.filename = tmp_filename;
    config_manage.filepath = tmp_filepath;

    size = optiway_exp__config_manage__get_packed_size(&config_manage);
    buf = x_malloc(size);

    if (buf == NULL)
        rv = OE_ERR_NO_MEMORY;
    else {
        optiway_exp__config_manage__pack(&config_manage, (uint8_t *)buf);
        nngx_fill_request(&req, "/config/export", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }

    return rv;
}

static int _rpc_oe_get_logo(void *h, ev_nngx_result_t *res, char *logo)
{
    int rv = OE_STATUS_OK;
    OptiwayExp__SysSoft *sys_soft = NULL;

    if (res->size != 0) {
        sys_soft = optiway_exp__sys_soft__unpack(NULL, ntohl(res->size), (uint8_t *)res->content);
        if (sys_soft == NULL) {
            rv = OE_ERR_NO_MEMORY;
        } else if (sys_soft->logo) {
            strcpy(logo, sys_soft->logo);
            optiway_exp__sys_soft__free_unpacked(sys_soft, NULL);
        } else {
            rv = OE_ERR_NETWORK_FALSE;
        }
    }

    return rv;
}

int rpc_oe_get_logo(void *h, char *logo)
{
    int rv = 0, temp_rv = 0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/system/logo", 0, NULL, E_NNGX_OP_GET);

    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        temp_rv = _rpc_oe_get_logo(h, res, logo);
        if (temp_rv != 0) {
            rv = temp_rv;
        }
    }

    x_free(res);

    return rv;
}
#endif

static int _rpc_oe_get_info(res_t *res, int *slot_id, int *platform)
{
    int rv = 0;
    mpack_reader_t reader;
    int head_type = 0;
    char str[128] = {0};

    mpack_reader_init(&reader, res->content, res->size, res->size);
    mpack_expect_array_match(&reader, 3);

    head_type = mpack_expect_i32(&reader);
    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    *slot_id = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_expect_map_match(&reader, 1);
    mpack_expect_str_buf(&reader,str,sizeof(str));
    *platform  = mpack_expect_i32(&reader);
    mpack_done_map(&reader);

    mpack_done_array(&reader);

	  printf("\r\n %s %d slot_id=%d platform=%d \n\r",__func__,__LINE__,*slot_id,*platform);
    if ((rv=mpack_reader_destroy(&reader)) != mpack_ok) {
        rv = OE_ERR_PARAM_ILLEGAL;
    }

    return rv;
}

int rpc_oe_get_info(int *slot_id, int *platform)
{
    int rv = 0, temp_rv = 0;
    string_buf sb;
    unsigned long long slot_mask = 0;
    res_t res0[MAX_SLOT_NUM];
    res_t *res[MAX_SLOT_NUM];
    int n_res = 0, i = 0;

    memset(&sb,0,sizeof(sb));

    for(i = 0; i < MAX_SLOT_NUM; i++){
        memset(&res0[i], 0, sizeof(res0[i]));
        res[i] = &res0[i];
    }

    rv = rpc_process_msg(&sb,slot_mask,OPCODE_SYSTEM_GET_PLATFORM,res,&n_res);
    if (rv == 0) {
        for(i = 0; i < n_res; i++){
            if(res[i]->rv == 0){
                if(res[i]->content){
                    temp_rv = _rpc_oe_get_info(res[i], slot_id, platform);
                    if (temp_rv != 0) {
                        rv = temp_rv;
                    }
                    free(res[i]->content);
                }
            }
        }
    }

    return rv;
}

#if 0
int rpc_oe_set_sysname(void *h, int slot, char *sysname)
{
    int rv = 0, size = 0;
    OptiwayExp__SysSoft sys_soft = OPTIWAY_EXP__SYS_SOFT__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    sys_soft.has_slot_id = 1;
    sys_soft.slot_id = slot;
    sys_soft.device_name = sysname;

    size = optiway_exp__sys_soft__get_packed_size(&sys_soft);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_soft__pack(&sys_soft, (uint8_t *)buf);
        nngx_fill_request(&req, "/sysname/set", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_set_datetime(void *h, char *datetime)
{
    int rv = 0, size = 0;
    OptiwayExp__SysSoft sys_soft = OPTIWAY_EXP__SYS_SOFT__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_soft.sys_time = datetime;

    size = optiway_exp__sys_soft__get_packed_size(&sys_soft);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_soft__pack(&sys_soft, (uint8_t *)buf);
        nngx_fill_request(&req, "/datetime/set", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

static int _rpc_oe_get_datetime(void *h, ev_nngx_result_t *res, char *datetime)
{
    int rv = OE_STATUS_OK;
    OptiwayExp__SysSoft *sys_soft = NULL;

    if (res->size != 0){
        sys_soft = optiway_exp__sys_soft__unpack(NULL, ntohl(res->size), (uint8_t *)res->content);
        if (sys_soft == NULL) {
            rv = OE_ERR_NO_MEMORY;
        } else if (sys_soft->sys_time != NULL) {
            strcpy(datetime, sys_soft->sys_time);
            optiway_exp__sys_soft__free_unpacked(sys_soft, NULL);
        } else {
            optiway_exp__sys_soft__free_unpacked(sys_soft, NULL);
            rv = OE_ERR_NETWORK_FALSE;
        }
    }

    return rv;
}

int rpc_oe_get_datetime(void *h, char *datetime)
{
    int rv = 0, temp_rv = 0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/datetime/get", 0, NULL, E_NNGX_OP_GET);
    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        temp_rv = _rpc_oe_get_datetime(h, res, datetime);
        if (temp_rv != 0) {
            rv = temp_rv;
        }
    }
    x_free(res);

    return rv;
}

int rpc_oe_set_timezone(void *h, char *timezone)
{
    int rv = 0, size = 0;
    OptiwayExp__SysSoft sys_soft = OPTIWAY_EXP__SYS_SOFT__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_soft.sys_time = timezone;

    size = optiway_exp__sys_soft__get_packed_size(&sys_soft);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_soft__pack(&sys_soft, (uint8_t *)buf);
        nngx_fill_request(&req, "/timezone/set", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_reset_board(void *h, int slot_id)
{
    int rv = 0, size = 0;
    OptiwayExp__SysReq sys_req = OPTIWAY_EXP__SYS_REQ__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_req.has_slot_id = 1;
    sys_req.slot_id = slot_id;

    size = optiway_exp__sys_req__get_packed_size(&sys_req);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_req__pack(&sys_req, (uint8_t *)buf);
        nngx_fill_request(&req, "/board/reset", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

#if 0
int rpc_oe_add_new_user(void *h, char *username, char *passwd, int level)
{
    int rv = 0, size =0;
    char tmp_login[MAX_USER_NAME_LEN] = {0};
    char tmp_pwd[256] = {0};

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo = OPTIWAY_EXP__USER_INFO__INIT;


    strcpy(tmp_login, username);
    tmp_login[strlen(username)] = 0;
    strcpy(tmp_pwd, passwd);
    tmp_pwd[strlen(passwd)] = 0;

    userinfo.login = tmp_login;
    userinfo.passwd= tmp_pwd;
    userinfo.has_level = 1;
    userinfo.level = level;
    userinfo.has_active= 1;
    userinfo.active = 1;

    size = optiway_exp__user_info__get_packed_size(&userinfo);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo, (uint8_t *)buf);

        nngx_fill_request(&req, "/user/add", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }

    return rv;
}

int rpc_oe_del_user(void *h, char * username)
{
    int rv = 0, size =0;
    char tmp_login[MAX_USER_NAME_LEN] = {0};

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo = OPTIWAY_EXP__USER_INFO__INIT;


    strcpy(tmp_login, username);
    tmp_login[strlen(username)] = 0;
    userinfo.login = tmp_login;

    size = optiway_exp__user_info__get_packed_size(&userinfo);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo, (uint8_t *)buf);

        nngx_fill_request(&req, "/user/del", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }

    return rv;
}

int rpc_oe_set_user_password(void *h, char *username, char *pwd)
{
    int rv = 0, size =0;
    char tmp_login[MAX_USER_NAME_LEN] = {0};
    char tmp_pwd[256] = {0};

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo = OPTIWAY_EXP__USER_INFO__INIT;


    strcpy(tmp_login, username);
    tmp_login[strlen(username)] = 0;
    strcpy(tmp_pwd, pwd);
    tmp_pwd[strlen(pwd)] = 0;

    userinfo.login = tmp_login;
    userinfo.passwd= tmp_pwd;

    size = optiway_exp__user_info__get_packed_size(&userinfo);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo, (uint8_t *)buf);

        nngx_fill_request(&req, "/user/set/pwd", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }

    return rv;
}

int rpc_oe_set_user_level(void *h, char *username, int level)
{
    int rv = 0, size =0;
    char tmp_login[MAX_USER_NAME_LEN] = {0};
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo = OPTIWAY_EXP__USER_INFO__INIT;


    strcpy(tmp_login, username);
    tmp_login[strlen(username)] = 0;
    userinfo.login = tmp_login;
    userinfo.has_level = 1;
    userinfo.level = level;

    size = optiway_exp__user_info__get_packed_size(&userinfo);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo, (uint8_t *)buf);

        nngx_fill_request(&req, "/user/set/level", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    return rv;
}

int rpc_oe_set_user_active(void *h, char *username, int active)
{
    int rv = 0, size =0;
    char tmp_login[MAX_USER_NAME_LEN] = {0};
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo = OPTIWAY_EXP__USER_INFO__INIT;


    strcpy(tmp_login, username);
    tmp_login[strlen(username)] = 0;
    userinfo.login = tmp_login;
    userinfo.has_active = 1;
    userinfo.active = active;

    size = optiway_exp__user_info__get_packed_size(&userinfo);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo, (uint8_t *)buf);

        nngx_fill_request(&req, "/user/set/active", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    return rv;
}

static int _rpc_oe_get_users_info(ev_nngx_result_t *res, struct oe_users_info_t *users_info)
{
    int i;
    int rv = 0;
    OptiwayExp__UsersInfo *users_res = NULL;

    if (res->size != 0) {
        users_res = optiway_exp__users_info__unpack(NULL, res->size, (uint8_t *)res->content);
        if (users_res == NULL) {
            rv = OE_ERR_NO_MEMORY;
        } else {
            if (users_res->user_sfp != NULL) {
                users_info->user_sfp = (struct user_info_t **)calloc(users_res->n_user_sfp, sizeof(struct user_info_t));
                if (users_info->user_sfp == NULL)
                    return OE_ERR_NO_MEMORY;
                for (i = 0; i < users_res->n_user_sfp; i++) {
                    if (users_res->user_sfp[i] != NULL) {
                        users_info->user_sfp[i] = (struct user_info_t *)malloc(sizeof(struct user_info_t));
                        strcpy( users_info->user_sfp[i]->login, users_res->user_sfp[i]->login );
                        users_info->user_sfp[i]->level = users_res->user_sfp[i]->level;
                        users_info->user_sfp[i]->active= users_res->user_sfp[i]->active;
                    }
                }
                users_info->n_user_sfp = users_res->n_user_sfp;
            }
        }
        optiway_exp__users_info__free_unpacked(users_res, NULL);
    }

    return rv;
}

int rpc_oe_show_users(void *h, int level, struct oe_users_info_t *users_info)
{
    int rv = 0, size =0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UserInfo userinfo_q = OPTIWAY_EXP__USER_INFO__INIT;

    userinfo_q.has_level = 1;
    userinfo_q.level = level;


    size = optiway_exp__user_info__get_packed_size(&userinfo_q);
    void *buf = NULL;
    buf = x_malloc(size);

    if (buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__user_info__pack(&userinfo_q, (uint8_t *)buf);
        nngx_fill_request(&req, "/user/show/users", size, buf);

        rv = nngx_send_and_recv(h, req, &res);

        if (rv == 0) {
            rv = _rpc_oe_get_users_info(res,users_info);
        }

        x_free(res);
        x_free(buf);
    }
    return rv;
}

int rpc_oe_free_users_info(struct oe_users_info_t *users_info)
{
    assert(users_info != NULL);

    int i = 0;

    if(users_info != NULL)
    {
        for(i = 0; i < users_info->n_user_sfp; i++)
            x_free(users_info->user_sfp[i]);
        x_free(users_info->user_sfp);
        users_info->user_sfp = NULL;
    }

    return 0;
}
#endif

int rpc_oe_ipconfig(void *h, int ip, int mask)
{
    int rv = 0, size = 0;
    OptiwayExp__SysReq sys_req = OPTIWAY_EXP__SYS_REQ__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_req.has_ip = 1;
    sys_req.ip = ip;
    sys_req.has_mask = 1;
    sys_req.mask = mask;

    size = optiway_exp__sys_req__get_packed_size(&sys_req);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_req__pack(&sys_req, (uint8_t *)buf);
        nngx_fill_request(&req, "/ipconfig", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_gateway(void *h, int ip)
{
    int rv = 0, size = 0;
    OptiwayExp__SysReq sys_req = OPTIWAY_EXP__SYS_REQ__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_req.has_ip = 1;
    sys_req.ip = ip;

    size = optiway_exp__sys_req__get_packed_size(&sys_req);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_req__pack(&sys_req, (uint8_t *)buf);
        nngx_fill_request(&req, "/gateway", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_opmode_set(void *h, int mode)
{
    int rv = 0, size = 0;
    OptiwayExp__SysReq sys_req = OPTIWAY_EXP__SYS_REQ__INIT;
    void *buf = NULL;
    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;


    sys_req.has_mode = 1;
    sys_req.mode = mode;

    size = optiway_exp__sys_req__get_packed_size(&sys_req);
    buf = x_malloc(size);
    if (buf != NULL) {
        optiway_exp__sys_req__pack(&sys_req, (uint8_t *)buf);
        nngx_fill_request(&req, "/set/opmode", size, buf, E_NNGX_OP_SET);

        rv = nngx_send_and_recv(h, req, &res);

        x_free(res);
        x_free(buf);
    }
    else {
        rv = OE_ERR_NO_MEMORY;
    }

    return rv;
}

int rpc_oe_rcload_bcm_config(void *h, int slot, char *filename)
{
    int rv = 0;
    int size = 0;
    void *buf = NULL;
    char tmpfilename[256] = {0};

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__SysSoft sys_soft = OPTIWAY_EXP__SYS_SOFT__INIT;

    strcpy(tmpfilename, filename);
    sys_soft.filename = tmpfilename;
    sys_soft.has_slot_id = 1;
    sys_soft.slot_id = slot;

    size = optiway_exp__sys_soft__get_packed_size(&sys_soft);
    buf = x_malloc(size);
    if(buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__sys_soft__pack(&sys_soft, (uint8_t *)buf);
        nngx_fill_request(&req, "/rcload/bcm/config", size, buf, E_NNGX_OP_SET);
        rv = nngx_send_and_recv(h, req, &res);
        x_free(res);
        x_free(buf);
    }
    return rv;
}

static int _rpc_oe_get_used_info(void *h, ev_nngx_result_t *res, rpc_used_info_t *used_info)
{
    int rv = OE_STATUS_OK;
    int i;
    OptiwayExp__UsedInfo *usedinfo = NULL;

    if (res->size != 0){
        usedinfo = optiway_exp__used_info__unpack(NULL, ntohl(res->size), (uint8_t *)res->content);
        if (usedinfo == NULL) {
            rv = OE_ERR_NO_MEMORY;
        } else if (usedinfo->sbpmap != NULL) {
            if (used_info->info) {
                used_info->info->slot = usedinfo->sbpmap[0]->slot;
                used_info->info->n_count = usedinfo->sbpmap[0]->n_bpma;
                for (i = 0; i < used_info->info->n_count; i++) {
                    used_info->info[0].bpmap[i].board_type = usedinfo->sbpmap[0]->bpma[i]->board_type;
                    used_info->info[0].bpmap[i].rio_type = usedinfo->sbpmap[0]->bpma[i]->rio_type;
                    used_info->info[0].bpmap[i].module = usedinfo->sbpmap[0]->bpma[i]->module;
                    used_info->info[0].bpmap[i].unit = usedinfo->sbpmap[0]->bpma[i]->unit;
                    used_info->info[0].bpmap[i].port_number = usedinfo->sbpmap[0]->bpma[i]->port_num;
                    used_info->info[0].bpmap[i].port_mask = usedinfo->sbpmap[0]->bpma[i]->port_mask;
                    memcpy(used_info->info[0].bpmap[i].port_map,
                        usedinfo->sbpmap[0]->bpma[i]->port_map, MAX_PORT_NUMBER * sizeof(int));
                }
            }
            optiway_exp__used_info__free_unpacked(usedinfo, NULL);
        } else {
            optiway_exp__used_info__free_unpacked(usedinfo, NULL);
            rv = OE_ERR_NETWORK_FALSE;
        }
    }

    return rv;
}

int rpc_oe_get_used_info(void *h, rpc_used_info_t *used_info)
{
    int rv = 0, temp_rv = 0;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;

    nngx_fill_request(&req, "/usedinfo/get", 0, NULL, E_NNGX_OP_GET);
    rv = nngx_send_and_recv(h, req, &res);
    if (rv == 0) {
        temp_rv = _rpc_oe_get_used_info(h, res, used_info);
        if (temp_rv != 0) {
            rv = temp_rv;
        }
    }
    x_free(res);

    return rv;
}

int rpc_oe_set_used_info(void *h, rpc_used_info_t *used_info)
{
    int rv = 0, i, j;
    int size = 0;
    void *buf = NULL;

    ev_nngx_request_t *req = NULL;
    ev_nngx_result_t *res = NULL;
    OptiwayExp__UsedInfo usedinfo = OPTIWAY_EXP__USED_INFO__INIT;

    usedinfo.n_sbpmap = used_info->n_info;
    usedinfo.sbpmap = calloc(usedinfo.n_sbpmap, sizeof(OptiwayExp__SlotBpmp *));
    if (!usedinfo.sbpmap) {
        return OE_ERR_NO_MEMORY;
    }
    for (i = 0; i < usedinfo.n_sbpmap; i++) {
        usedinfo.sbpmap[i] = calloc(1, sizeof(OptiwayExp__SlotBpmp));
        if (!usedinfo.sbpmap[i]) {
            break;
        }

        optiway_exp__slot_bpmp__init(usedinfo.sbpmap[i]);
        usedinfo.sbpmap[i]->slot = used_info->info[i].slot;
        usedinfo.sbpmap[i]->n_bpma = used_info->info[i].n_count;
        usedinfo.sbpmap[i]->bpma = calloc(1, sizeof(OptiwayExp__Bpmap *));
        if (usedinfo.sbpmap[i]->bpma) {
            for (j = 0; j < usedinfo.sbpmap[i]->n_bpma; j++) {
                usedinfo.sbpmap[i]->bpma[j] = calloc(1, sizeof(OptiwayExp__Bpmap));
                if (!usedinfo.sbpmap[i]->bpma[j]) {
                    break;
                }

                optiway_exp__bpmap__init(usedinfo.sbpmap[i]->bpma[j]);
                usedinfo.sbpmap[i]->bpma[j]->board_type = used_info->info[i].bpmap[j].board_type;
                usedinfo.sbpmap[i]->bpma[j]->rio_type = used_info->info[i].bpmap[j].rio_type;
                usedinfo.sbpmap[i]->bpma[j]->module = used_info->info[i].bpmap[j].module;
                usedinfo.sbpmap[i]->bpma[j]->unit = used_info->info[i].bpmap[j].unit;
                usedinfo.sbpmap[i]->bpma[j]->port_num = used_info->info[i].bpmap[j].port_number;
                usedinfo.sbpmap[i]->bpma[j]->port_mask = used_info->info[i].bpmap[j].port_mask;
                usedinfo.sbpmap[i]->bpma[j]->n_port_map = MAX_PORT_NUMBER;
                usedinfo.sbpmap[i]->bpma[j]->port_map = malloc(MAX_PORT_NUMBER * sizeof(int32_t));
                if (usedinfo.sbpmap[i]->bpma[j]->port_map) {
                    memcpy(usedinfo.sbpmap[i]->bpma[j]->port_map,
                        used_info->info[i].bpmap[j].port_map, MAX_PORT_NUMBER * sizeof(int32_t));
                }
            }
            usedinfo.sbpmap[i]->n_bpma = j;
        }
    }
    usedinfo.n_sbpmap = i;

    size = optiway_exp__used_info__get_packed_size(&usedinfo);
    buf = x_malloc(size);
    if(buf == NULL) {
        rv = OE_ERR_NO_MEMORY;
    } else {
        optiway_exp__used_info__pack(&usedinfo, (uint8_t *)buf);
        nngx_fill_request(&req, "/usedinfo/set", size, buf, E_NNGX_OP_SET);
        rv = nngx_send_and_recv(h, req, &res);
        x_free(res);
        x_free(buf);
    }
    for (i = 0; i < usedinfo.n_sbpmap; i++) {
        if (usedinfo.sbpmap[i]) {
            for (j = 0; j < usedinfo.sbpmap[i]->n_bpma; j++) {
                if (usedinfo.sbpmap[i]->bpma[j]) {
                    if (usedinfo.sbpmap[i]->bpma[j]->port_map) {
                        free(usedinfo.sbpmap[i]->bpma[j]->port_map);
                    }
                    free(usedinfo.sbpmap[i]->bpma[j]);
                }
            }
            free(usedinfo.sbpmap[i]);
        }
    }
    free(usedinfo.sbpmap);
    return rv;
}
#endif
