#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "string_buf.h"
#include "js.h"
#include "nngn_struct.h"
#include "nngn.h"
#include "error_code.h"
#include "get_special_param.h"
#include "mpack-config.h"	
#include "mpack.h"
#include "srv_mpack_common.h"
#include "appdefs.h"
#include "rpc_def.h"

static int port_link_flag = 0;

static int _get_smp_buf(int slot, int module, int port, char *buf)
{
    char module_str[2] = {0};

    if(module == 4){
        strcpy(module_str,"r");
    }else if(module == 5){
        strcpy(module_str,"f");
    }else if(module == 6){
        strcpy(module_str,"b");
    }

    if(port == 0){ sprintf(buf,"%d/%s",slot,module_str);
    }else{
        sprintf(buf,"%d/%s/%d",slot,module_str,port);
    }

    return 0; 
}

static int unpack_smp(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    mpack_tag_t tag_child;
    char smp[128] = {0};
    int slot = 0, module = 0, port = 0;
        
    tag = mpack_read_tag(reader);
    switch(tag.v.n){
        case 1:
            tag_child = mpack_read_tag(reader);
            fprintf(stderr,"%-20ld ",tag_child.v.u);
            break;
        case 2:
            tag_child = mpack_read_tag(reader);
            slot = tag_child.v.u;
            tag_child = mpack_read_tag(reader);
            module = tag_child.v.u;
            _get_smp_buf(slot,module,0,smp);
            fprintf(stderr,"%-20s ",smp);
            break;
        case 3:
            tag_child = mpack_read_tag(reader);
            slot = tag_child.v.u;
            tag_child = mpack_read_tag(reader);
            module = tag_child.v.u;
            tag_child = mpack_read_tag(reader);
            port = tag_child.v.u;
            _get_smp_buf(slot,module,port,smp);
            fprintf(stderr,"%-20s ",smp);
            break;
    }

    return 0;
}

static int unpack_uptime(mpack_reader_t *reader)
{
    int day = 0, hour = 0, min = 0, sec = 0;
    char up_time[128] = {0};

    mpack_expect_array_match(reader, 4);
    day = mpack_expect_u64(reader);
    hour = mpack_expect_u64(reader);
    min = mpack_expect_u64(reader);
    sec = mpack_expect_u64(reader);
    mpack_done_array(reader);

    if(day > 1){
        sprintf(up_time,"%d days %02dhour %02dmin %02dsec",day,hour,min,sec);
    }else{
        sprintf(up_time,"%d day %02dhour %02dmin %02dsec",day,hour,min,sec);
    }
    fprintf(stderr,"%-20s ",up_time);

    return 0;
}

static int unpack_link(int link, int *link_flag)
{
    if(link){
        *link_flag = 1;
        fprintf(stderr,"%-15s ","up");
    }else{
        *link_flag = 0;
        fprintf(stderr,"%-15s ","down");
    }

    return 0;
}

static int unpack_switch(int switch_on_off)
{
    if(switch_on_off){
        fprintf(stderr,"%-15s ","on");
    }else{
        fprintf(stderr,"%-15s ","off");
    }

    return 0;
}

static int _fmt_speed(char *buf, int speed)
{
#if 0
    if (speed >= 1000)  {
        if (speed % 1000) {
             sprintf(buf, "%d.%dG", speed / 1000, (speed % 1000) / 1000);
         } else {
             sprintf(buf, "%dG", speed / 1000);
         }
    } else if (speed == 0) {
         sprintf(buf, "--");
    } else {
          sprintf(buf, "%dM", speed);
    }
#endif
	switch(speed){
		case 0:
			sprintf(buf, "--");
			break;
		case 1:
			sprintf(buf, "1G");
			break;
		case 2:
			sprintf(buf, "10G");
			break;
		case 3:
			sprintf(buf, "25G");
			break;
		case 4:
			sprintf(buf, "40G");
			break;
		case 5:
			sprintf(buf, "40G_");
			break;
		case 6:
			sprintf(buf, "50G");
			break;
		case 7:
			sprintf(buf, "100G");
			break;
		case 8:
			sprintf(buf, "40G__");
			break;
		default:
			break;
	}
    return 0;
}

static int unpack_speed(int speed, int link_flag)
{
    char buf[6] = {0};

    if(link_flag){
        _fmt_speed(buf,speed);
        fprintf(stderr,"%-15s ",buf);
    }else{
        fprintf(stderr,"%-15s ","--");
    }

    return 0;
}

static int unpack_duplex(int duplex, int link_flag)
{
    if(link_flag){
        if(duplex){
            fprintf(stderr,"%-15s ","half");
        }else{
            fprintf(stderr,"%-15s ","full");
        }
    }else{
        fprintf(stderr,"%-15s ","--");
    }

    return 0;
}

static int unpack_counter_type(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    uint64_t type = 0, total = 0, rate = 0;

    tag = mpack_read_tag(reader);
    type = tag.v.u;
    
    tag = mpack_read_tag(reader);
    total = tag.v.u;
    
    tag = mpack_read_tag(reader);
    rate = tag.v.u;

    switch (type & 0xf) {
        case 0:
            break;
        case 2:
            fprintf(stderr, "%lld%-20s %lld%-20s",total,"bytes",rate,"kbps");
            break;
        case 0xf:
            break;
        case 1:
        default:
            fprintf(stderr, "%lld%-20s %lld%-20s",total,"pkts",rate,"pps");
            break;
    }

    return 0;
}

static int unpack_ip(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    uint64_t sip = 0, sip_mask = 0;
    struct in_addr in;
        
    tag = mpack_read_tag(reader);
    
    tag = mpack_read_tag(reader); 
    sip = tag.v.u;
     
    in.s_addr = ntohl(sip);
    fprintf(stderr,"%s/",inet_ntoa(in));
    
    tag = mpack_read_tag(reader); 
    sip_mask = tag.v.u;
     
    in.s_addr = ntohl(sip_mask);
    fprintf(stderr,"%s ",inet_ntoa(in));
    return 0;
}

static int unpack_ipv6(mpack_reader_t *reader)
{
    int i = 0, j = 0;
    mpack_tag_t tag;
    char str_buf[16] = {0};
    unsigned short ipv6[8];
    unsigned short ipv6_mask[8];
    
    tag = mpack_read_tag(reader);
    
    tag = mpack_read_tag(reader); 
    mpack_read_bytes(reader, str_buf, tag.v.l);
    mpack_done_str(reader);
    for(i = 0; i < tag.v.l; i=i+2){
        ipv6[j] = ((unsigned char)str_buf[i+1] << 8) + (unsigned char)str_buf[i];
        ++j;
    }

    j = 0;
    memset(str_buf, 0, sizeof(str_buf));
    tag = mpack_read_tag(reader); 
    mpack_read_bytes(reader, str_buf, tag.v.l);
    mpack_done_str(reader);
    for(i = 0; i < tag.v.l; i=i+2){
        ipv6_mask[j] = ((unsigned char)str_buf[i+1] << 8) + (unsigned char)str_buf[i];
        ++j;
    }
    fprintf(stderr,"%x:%x:%x:%x:%x:%x:%x:%x/%x:%x:%x:%x:%x:%x:%x:%x ",
                ipv6[0],ipv6[1],ipv6[2],ipv6[3],
                ipv6[4],ipv6[5],ipv6[6],ipv6[7],
                ipv6_mask[0],ipv6_mask[1],ipv6_mask[2],ipv6_mask[3],
                ipv6_mask[4],ipv6_mask[5],ipv6_mask[6],ipv6_mask[7]);
    return 0;
}

static int unpack_mac(mpack_reader_t *reader)
{
    int i = 0;
    mpack_tag_t tag;
    char str_buf[16] = {0};
    unsigned char mac[6];
    unsigned char mac_mask[6];

    tag = mpack_read_tag(reader);
    
    tag = mpack_read_tag(reader); 
    mpack_read_bytes(reader, str_buf, tag.v.l);
    mpack_done_str(reader);
    
    for(i = 0; i < tag.v.l; i++){
        mac[i] = (unsigned char)str_buf[i];
    }

    memset(str_buf, 0, sizeof(str_buf));
    tag = mpack_read_tag(reader); 
    mpack_read_bytes(reader, str_buf, tag.v.l);
    mpack_done_str(reader);
    
    for(i = 0; i < tag.v.l; i++){
        mac_mask[i] = (unsigned char)str_buf[i];
    }

    fprintf(stderr,"%x:%x:%x:%x:%x:%x/%x:%x:%x:%x:%x:%x ",
            mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
            mac_mask[0],mac_mask[1],mac_mask[2],mac_mask[3],mac_mask[4],mac_mask[5]);
    return 0;
}

static int _Sprint_Members_From_Mask(char *buf, unsigned long long mask, int max)
{
    int port = 0, range = 0, printed = 0;
    char *buffer = buf;
    for (port = 0; port < max; port++)
        if (mask & (1ull << port)) {
            if (!range) {
                if (printed)
                    buffer += sprintf(buffer, ",");
                buffer += sprintf(buffer, "%d", port);

                printed = 1;

                if (mask & (1ull << (port + 1)))
                    range = port;
            }
        } else if (range) {
            buffer += sprintf(buffer, "-%d", port - 1);
            range = 0;
        }

    if (range)
        buffer += sprintf(buffer, "-%d", max - 1);

    return buffer - buf;
}

static int _check_port_in_ports(unsigned long long*p, int port)
{
    int index=0;
    int offset=0;   

    if(NULL==p) 
        return -1;

    if((port<0)||(port>MAX_PORT_NUMBER))
        return -1;

    index=port/64; 
    offset=port%64; 

    if(p[index] & (1ull<<offset))
        return 1;
    else
        return 0;
}   


static int _Sprint_Ports_From_Mask(char *buf, unsigned long long* mask)
{
    int port = 0, range = 0, printed = 0;
    char *buffer = buf;
    int max=MAX_PORT_NUMBER;
    for (port = 0; port < MAX_PORT_NUMBER; port++)
        if (_check_port_in_ports(mask,port)) {
            if (!range) {
                if (printed)
                    buffer += sprintf(buffer, ",");
                buffer += sprintf(buffer, "%d", port+1);
                printed = 1;

                if (_check_port_in_ports(mask, port+1))
                    range = port+1;
            }
        } else if (range) {
            buffer += sprintf(buffer, "-%d", port);
            range = 0;
        }

    if (range)
        buffer += sprintf(buffer, "-%d", max);

    return buffer - buf;
}

static int _check_ports_full(unsigned long long* ports)
{
    if(NULL==ports)
        return -1;

    if( (ports[0]==0xffffffffffffffffLL)
         && (ports[1]==0xffffffffffffffffLL))
        return 1;
    else
        return 0;
}


static int _print_smp_buf_new(int slots, int modules, unsigned long long*ports, char *smp_buf)
{
	char slot_buf[128] = {0};
	char module_buf[128] = {0};
	char port_buf[128] = {0};
	char *p = NULL;

	_Sprint_Members_From_Mask(slot_buf, slots << 1, SLOT_NUMBER_PER_SYSTEM + 1);
	_Sprint_Members_From_Mask(module_buf, modules, MODULE_BACK + 1);
	for (p = module_buf; p != NULL && *p != 0; p++) {
		if (*p == '4')
			*p = 'r';
		else if (*p == '5')
			*p = 'f';
        else if (*p == '6')
            *p = 'b';
	}
	_Sprint_Ports_From_Mask(port_buf, ports);

    if (_check_ports_full(ports)) {
        sprintf(smp_buf, "%s/%s/*", slot_buf, module_buf);
    } else {
	    sprintf(smp_buf, "%s/%s/%s", slot_buf, module_buf, port_buf);
    }
	return 0;
}

static int unpack_inports_outports(mpack_reader_t *reader)
{
    mpack_tag_t tag, tag_child, tag_s;
    int j = 0;
    unsigned long long ports[4];
    uint64_t slot = 0, module = 0, weight = 0;
    char smp_buf[1024] = {0};
        
    tag = mpack_read_tag(reader);
    for(j = 0; j < tag.v.n; j++){
        tag_child = mpack_read_tag(reader);

        tag_s =  mpack_read_tag(reader);  
        slot = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        module = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ports[0] = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ports[1] = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ports[2] = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ports[3] = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        weight = tag_s.v.u;
        
        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    _print_smp_buf_new(slot, module, ports, smp_buf);
    fprintf(stderr,"%s weight:%ld", smp_buf,weight);
    return 0;
}

static int unpack_user_data(mpack_reader_t *reader)
{
    mpack_tag_t tag, tag_child, tag_s;
    int j = 0;
    int ud_num = 0, ud_data = 0, ud_mask = 0;
        
    tag = mpack_read_tag(reader);
    for(j = 0; j < tag.v.n; j++){
        tag_child = mpack_read_tag(reader);

        tag_s =  mpack_read_tag(reader);  
        ud_num = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ud_data = tag_s.v.u;
        
        tag_s =  mpack_read_tag(reader);  
        ud_mask = tag_s.v.u;
        
        fprintf(stderr,"ud%d=0x%x/0x%x ", ud_num,ud_data,ud_mask);
        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    return 0;
}

static int unpack_power(mpack_reader_t *reader)
{
    mpack_tag_t tag, tag_child, tag_s;
    int j = 0;
    int power_id = 0, power_status = 0;
    
    tag = mpack_read_tag(reader);
    for(j = 0; j < tag.v.n; j++){
        tag_child = mpack_read_tag(reader);

        tag_s =  mpack_read_tag(reader);  
        power_id = tag_s.v.u;

        tag_s =  mpack_read_tag(reader);  
        power_status = tag_s.v.u;

        fprintf(stderr,"\n");
        if(power_status == STATUS_NORMAL){
            fprintf(stderr,"        id:%d status:normal ",power_id);
        }else if(power_status == STATUS_ABNORMAL){
            fprintf(stderr,"        id:%d status:abnormal ",power_id);
        }else if(power_status == STATUS_OFFLINE){
            fprintf(stderr,"        id:%d status:offline ",power_id);
        }

        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    return 0;
}

static int unpack_air_fan(mpack_reader_t *reader)
{
    mpack_tag_t tag, tag_child, tag_s;
    int j = 0;
    int air_id = 0, air_status = 0;
    
    tag = mpack_read_tag(reader);
    for(j = 0; j < tag.v.n; j++){
        tag_child = mpack_read_tag(reader);

        tag_s =  mpack_read_tag(reader);  
        air_id = tag_s.v.u;

        tag_s =  mpack_read_tag(reader);  
        air_status = tag_s.v.u;

        fprintf(stderr,"\n");
        if(air_status == STATUS_NORMAL){
            fprintf(stderr,"        id:%d status:normal ",air_id);
        }else if(air_status == STATUS_ABNORMAL){
            fprintf(stderr,"        id:%d status:abnormal ",air_id);
        }else if(air_status == STATUS_OFFLINE){
            fprintf(stderr,"        id:%d status:offline ",air_id);
        }

        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    return 0;
}

static int unpack_platform(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int platform = 0;
    
    tag = mpack_read_tag(reader);
    platform = tag.v.u;

    if(PLATFORM_S320 == platform){
        fprintf(stderr,"S320 ");
    }else if(PLATFORM_S40 == platform){
        fprintf(stderr,"S40 ");
    }
    
    return 0;
}

static int unpack_board_type(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int board_type = 0;
    num_str_t board_type_table[] = {
        {BOARD_TYPE_AC1200, "AC1200"},
        {BOARD_TYPE_SF3600, "SF3600"},
        {BOARD_TYPE_SF3600_SW, "SF3600_SW"},
        {BOARD_TYPE_SF3600_F16, "SF3600_F16"},
        {BOARD_TYPE_ST3600, "ST3600"},
        {BOARD_TYPE_ET5100, "ET5100"},
        {BOARD_TYPE_EP7000, "EP7000"},
        {BOARD_TYPE_EP7024, "EP7024"},
        {BOARD_TYPE_AC3800, "AC3800"},
        {BOARD_TYPE_EP7032, "EP7032"},
        {BOARD_TYPE_EP7048, "EP7048"},
        {BOARD_TYPE_EP7056, "EP7056"},
        {BOARD_TYPE_SF4800, "SF4800"},
        {BOARD_TYPE_AC2820, "AC2820"},
        {BOARD_TYPE_AC4300, "AC4300"},
        {BOARD_TYPE_EP8010, "EP8010"},
        {BOARD_TYPE_EP8020, "EP8020"},
		//add
		{BOARD_TYPE_EP8110, "EP8110"},
    };

    tag = mpack_read_tag(reader);
    board_type = tag.v.u;

    fprintf(stderr,"%s ",GetString(board_type_table, board_type, BOARD_TYPE_MAX));

    return 0;
}

static int unpack_load_banlance(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int load_balance = 0;
    char g_load_balance[][20] = {"", "smac", "dmac", "mac", "sip", "dip", "ip", "", "", "", "", ""};

    tag = mpack_read_tag(reader);
    load_balance = tag.v.u;

    fprintf(stderr,"%s ",g_load_balance[load_balance]);

    return 0;
}

static int unpack_vlan_tag(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int vlan_tag = 0;

    tag = mpack_read_tag(reader);
    vlan_tag = tag.v.u;

    if(vlan_tag){
        fprintf(stderr,"untag ");
    }else{
        fprintf(stderr,"tag ");
    }

    return 0;
}

static int unpack_action(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int action_type = 0;

    tag = mpack_read_tag(reader);
    action_type = tag.v.u;

    if(action_type == OUTPORT_MC){
        fprintf(stderr,"mc ");
    }else if(action_type == OUTPORT_HASH){
        fprintf(stderr,"hash ");
    }

    return 0;
}

static int unpack_fw_type(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int fw_type = 0;

    tag = mpack_read_tag(reader);
    fw_type = tag.v.u;

    if(fw_type == FW_MODE_DROP){
        fprintf(stderr,"fw drop ");
    }else{
        fprintf(stderr,"fw outports ");
    }

    return 0;
}

static int unpack_out_vlan(mpack_reader_t *reader)
{
    int out_vlan = 0, vlan_min = 0, vlan_max = 0;

    mpack_expect_array_match(reader,3);
    out_vlan = mpack_expect_u64(reader);
    vlan_min = mpack_expect_u64(reader);
    vlan_max = mpack_expect_u64(reader);
    mpack_done_array(reader);

    if(out_vlan){
        fprintf(stderr,"%d",out_vlan);
    }else if(vlan_max > vlan_min){
        fprintf(stderr,"%d-%d",vlan_min,vlan_max);
    }

    return 0;
}

static int unpack_om_switch_over(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int om_switch = 0;

    tag = mpack_read_tag(reader);
    om_switch = tag.v.u;

    if(om_switch == 0){
        fprintf(stderr,"slave");
    }else{
        fprintf(stderr,"master");
    }

    return 0;
}

static int unpack_board_status(mpack_reader_t *reader)
{
    mpack_tag_t tag;
    int board_status = 0;
    static num_str_t board_sts_table[]={
        {BD_STATUS_BOARD_NOT_EXSIT,                             "offline"},
        {BD_STATUS_CONFIG_SUCCEED,                            "normal"},
        {BD_STATUS_CONFIG_AND_CONFIG_FAILED,              "abnormal"},
    };

    tag = mpack_read_tag(reader);
    board_status = tag.v.u;

    if(board_status >= BD_STATUS_BOARD_NOT_EXSIT && board_status <= BD_STATUS_CONFIG_SUCCEED){
        fprintf(stderr,"%s",board_sts_table[board_status].str);
    }else{
        fprintf(stderr,"INVALID PARAMETER");
    }

    return 0;
}



static int unpack_spec_val(char *spec_key_str,mpack_reader_t *reader, int val, char *str_val)
{
    int rv = 0;

    if(strcmp(spec_key_str,"\%s") == 0){
        unpack_smp(reader);
    }else if(strcmp(spec_key_str,"\%D") == 0){
        unpack_uptime(reader);
    }else if(strcmp(spec_key_str,"%l") == 0){
        unpack_link(val,&port_link_flag);
    }else if(strcmp(spec_key_str,"\%o") == 0){
        unpack_switch(val);
    }else if(strcmp(spec_key_str,"\%S") == 0){
        unpack_speed(val,port_link_flag);
    }else if(strcmp(spec_key_str,"\%d") == 0){
        unpack_duplex(val,port_link_flag);
    }else if(strcmp(spec_key_str,"\%i") == 0){
        unpack_ip(reader);
    }else if(strcmp(spec_key_str,"%I") == 0){
        unpack_ipv6(reader);
    }else if(strcmp(spec_key_str,"%m") == 0){
        unpack_mac(reader);
    }else if(strcmp(spec_key_str,"\%c") == 0){
        unpack_counter_type(reader);
    }else if(strcmp(spec_key_str,"\%p") == 0){
        unpack_inports_outports(reader);
    }else if(strcmp(spec_key_str,"\%u") == 0){
        unpack_user_data(reader);
    }else if(strcmp(spec_key_str,"%P") == 0){
        unpack_power(reader);
    }else if(strcmp(spec_key_str,"\%a") == 0){
        unpack_air_fan(reader);
    }else if(strcmp(spec_key_str,"\%f") == 0){
        unpack_platform(reader);
    }else if(strcmp(spec_key_str,"\%b") == 0){
        unpack_board_type(reader);
    }else if(strcmp(spec_key_str,"%L") == 0){
        unpack_load_banlance(reader);
    }else if(strcmp(spec_key_str,"%v") == 0){
        unpack_vlan_tag(reader);
    }else if(strcmp(spec_key_str,"\%A") == 0){
        unpack_action(reader);
    }else if(strcmp(spec_key_str,"\%F") == 0){
        unpack_fw_type(reader);
    }else if(strcmp(spec_key_str,"%V") == 0){
        unpack_out_vlan(reader);
    }else if(strcmp(spec_key_str,"\%O") == 0){
        unpack_om_switch_over(reader);
    }else if(strcmp(spec_key_str,"\%t") == 0){
        unpack_board_status(reader);
    }

    return rv;
}

static int unpack_by_type(mpack_tag_t *tag, mpack_reader_t *reader, char *table_head, int depth)
{
    int rv = 0;
    int j = 0;
    mpack_tag_t tag_child;
    char *spec_key_str = NULL, *real_key = NULL;
    char key_str[128] = {0};
    char *str = NULL;
    char depth_str[128] = {0};
    int len = 0;

    if(depth == 0){
        sprintf(depth_str,"    ");
    }else if(depth == 1){
        sprintf(depth_str,"        ");
    }

    if(tag->type == mpack_type_uint){
        if(table_head == NULL || strlen(table_head) == 0){
            fprintf(stderr,"%-7ld ",tag->v.u);
        }else{
            unpack_spec_val(table_head,reader,tag->v.u,NULL);
        }
    }else if(tag->type == mpack_type_str){
        len = 0;
        len = tag->v.l+1;
        str = (char *)malloc(len);
        memset(str,0,len);
        mpack_read_bytes(reader, str, tag->v.l);
        mpack_done_str(reader);
        if(table_head == NULL || strlen(table_head) == 0){
          fprintf(stderr,"%-20s ",str);
        }else{
            unpack_spec_val(table_head,reader,0,str);
        }
        if(str){
            free(str);
        }
    }else if(tag->type == mpack_type_array){
        if(tag->v.n != 0){
            if(table_head == NULL || strlen(table_head) == 0){
                for(j = 0; j < tag->v.n; j++){
                    tag_child = mpack_read_tag(reader);
                    unpack_by_type(&tag_child,reader,NULL,depth);
                }
            }else{
                unpack_spec_val(table_head,reader,0,NULL);
            }
        }else{
            fprintf(stderr,"%s-- ",depth_str);
        }
        mpack_done_array(reader);
    }else if(tag->type == mpack_type_map){
        for(j = 0; j < tag->v.n; j++){
            //unpack key
            tag_child = mpack_read_tag(reader);
            if(tag_child.type != mpack_type_str){
                //unpack key
                unpack_by_type(&tag_child,reader,NULL,depth);

                //unpack val
                tag_child = mpack_read_tag(reader);
                unpack_by_type(&tag_child,reader,NULL,depth);
            }else{
                memset(key_str, 0, 128);
                mpack_read_bytes(reader, key_str, tag_child.v.l);
                mpack_done_str(reader);
                if((spec_key_str = strstr(key_str,"%")) != NULL){
                    char str_array[128];
                    strcpy(str_array,key_str);
                    char *tmp_str = str_array;
                    real_key = strsep(&tmp_str,"%");
                    fprintf(stderr,"%s%s: ",depth_str,real_key);
                    unpack_spec_val(spec_key_str,reader,0,NULL);
                }else{
                    fprintf(stderr,"%s%s: ",depth_str,key_str);
                    //unpack val
                    tag_child = mpack_read_tag(reader);
                    unpack_by_type(&tag_child,reader,NULL,depth);
                }
            }
        }
        mpack_done_map(reader);
    }

    return rv;
}

static int show_simple_dimensional_without_head(mpack_reader_t *reader,int ele_num)
{
    int rv = 0;
    int i = 0;
    mpack_tag_t tag;
    int depth = 0;
    
    for(i = ele_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){
        tag = mpack_read_tag(reader);
        unpack_by_type(&tag,reader,NULL,depth);
        fprintf(stderr,"\n");
    }
    mpack_done_array(reader);

    return rv;
}

static int show_simple_dimensional_with_print_head(mpack_reader_t *reader, int list_num, int ele_num)
{
    int rv = 0;
    int i = 0, j = 0;
    char list_name[128] = {0},str_array[128];
    char *tmp_str = NULL;
    char head[list_num][10];
    char *spec_key_str = NULL, *real_list_name = NULL;
    mpack_tag_t tag, child_tag;

    for(i = 0; i < list_num; i++){
        memset(head[i], 0 , sizeof(head[i]));
    }

    //for head
    for(i = list_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){ 
        memset(list_name, 0, 128);
        mpack_expect_str_buf(reader,list_name,sizeof(list_name));
        if((spec_key_str = strstr(list_name,"%")) != NULL){
            memset(str_array, 0, 128);
            strcpy(str_array,list_name);
            tmp_str = str_array;
            real_list_name = NULL;
            real_list_name = strsep(&tmp_str,"%");
            fprintf(stderr,"%-15s ",real_list_name);
            strcpy(head[list_num-i],spec_key_str);
        }else{
            fprintf(stderr,"%-15s ",list_name);
        }
    }
    fprintf(stderr,"\n");
    fprintf(stderr,"----------------------------------------------------------------------------\n");
    mpack_done_array(reader);

    for(i = ele_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){
        tag = mpack_read_tag(reader);
        for(j = 0; j < tag.v.n; j++){
            child_tag = mpack_read_tag(reader);
            unpack_by_type(&child_tag,reader,head[j],0);
        }
        fprintf(stderr,"\n");
        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    return rv;
}

static int show_mul_dimensional_with_not_print_head(mpack_reader_t *reader, int list_num, int ele_num)
{
    int rv = 0;
    int i = 0, j = 0;
    char list_name[128] = {0};
    char head[list_num][10];
    char *spec_key_str = NULL;
    mpack_tag_t tag, child_tag;

    for(i = 0; i < list_num; i++){
        memset(head[i], 0 , sizeof(head[i]));
    }

    //for head
    for(i = list_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){ 
        memset(list_name, 0, 128);
        mpack_expect_str_buf(reader,list_name,sizeof(list_name));
        if((spec_key_str = strstr(list_name,"%")) != NULL){
            strcpy(head[list_num-i],spec_key_str);
        }
    }
    fprintf(stderr,"\n");
    fprintf(stderr,"----------------------------------------------------------------------------\n");
    mpack_done_array(reader);

    for(i = ele_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){
        tag = mpack_read_tag(reader);
        for(j = 0; j < tag.v.n; j++){
            child_tag = mpack_read_tag(reader);
            unpack_by_type(&child_tag,reader,head[j],1);
        }
        fprintf(stderr,"\n");
        mpack_done_array(reader);
    }
    fprintf(stderr,"----------------------------------------------------------------------------\n");
    fprintf(stderr,"\n");
    mpack_done_array(reader);

    return rv;
}

static int show_mul_dimensional_without_head(mpack_reader_t *reader, int ele_num)
{
    int rv = 0;
    int i = 0, j = 0, k = 0;
    mpack_tag_t tag, tag_child;
    int domain_ele_num = 0, num = 0;
    
    for(i = ele_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){
	    domain_ele_num = mpack_expect_array(reader);
        //unpack domain ele
        tag = mpack_read_tag(reader);
        unpack_by_type(&tag,reader,NULL,0);
        fprintf(stderr,"\n");
	
        num = domain_ele_num - 1;
		if(num > 0){
            fprintf(stderr,"----------------------------------------------------------------------------\n");
        }
        for(j = num; j > 0 && mpack_reader_error(reader) == mpack_ok; j--){
            tag_child = mpack_read_tag(reader);
            for(k = 0; k < tag_child.v.n; k++){
                unpack_by_type(&tag_child,reader,NULL,1);
            }
            fprintf(stderr,"\n");
        }

        if(num > 0){
            fprintf(stderr,"----------------------------------------------------------------------------\n");
        }
        fprintf(stderr,"\n");

        mpack_done_array(reader);
    }
    mpack_done_array(reader);


    return rv;
}

#if 0
static int printf_int_key_val_by_index(int c_depth,int l_depth, mpack_reader_t *reader,char *table_head)
{
    int j = 0, m = 0;
    mpack_tag_t tag;

    for (j = l_depth; j < c_depth - 1; ++j)
    {
        fprintf(stderr,"--    ");
    }

    if(strlen(table_head) > 0){
        unpack_spec_val(table_head,reader,0,NULL);
    }else{
        tag = mpack_read_tag(reader);
        unpack_by_type(&tag,reader,NULL,0);
    }

    return 0;
} 
#else
static int printf_int_key_val_by_index(int key, mpack_reader_t *reader,char *table_head)
{
    mpack_tag_t tag;
    char *key_str[KEY_COUNT - 1] = {"add rule"/*"rule_id"*/,"protocol","ethertype","sip","dip",
                                    "sipv6","dipv6","smac","dmac","sport","dport",
                                    "tcpflag","ud"};

    if(strcmp(key_str[key-1],"ud") != 0){
	/***************************这里是加的*********/
		if(strcmp(key_str[key-1], "add rule") == 0)
		{
			fprintf(stderr, "%s ",key_str[key-1]);
		}
		else
	/************************************/		
			fprintf(stderr,"%s=",key_str[key-1]);
    }
    if(strlen(table_head) > 0){
        unpack_spec_val(table_head,reader,0,NULL);
    }else{
        tag = mpack_read_tag(reader);
        unpack_by_type(&tag,reader,NULL,0);
    }

    return 0;
} 
#endif

static int show_simple_dimensional_with_sparse_head(mpack_reader_t *reader, int list_num, int ele_num)
{
    int rv = 0;
    int i = 0, j = 0;
    char list_name[128] = {0};
    //char *tmp_str = NULL;
    char head[list_num][10];
    char *spec_key_str = NULL;
    mpack_tag_t tag, child_tag;
    //int key_used[list_num];
    int key = 0;
    //int l_c_depth = 0;

    for(i = 0; i < list_num; i++){
        memset(head[i], 0 , sizeof(head[i]));
    }

    //for head
    for(i = list_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){ 
        memset(list_name, 0, 128);
        mpack_expect_str_buf(reader,list_name,sizeof(list_name));
        if((spec_key_str = strstr(list_name,"%")) != NULL){
            #if 0
            memset(str_array, 0, 128);
            strcpy(str_array,list_name);
            tmp_str = str_array;
            real_list_name = NULL;
            real_list_name = strsep(&tmp_str,"%");
            fprintf(stderr,"%s    ",real_list_name);
            #endif
            strcpy(head[list_num-i],spec_key_str);
        #if 0
        }else{
            fprintf(stderr,"%s    ",list_name);
        #endif
        }
    }
//hp屏蔽    fprintf(stderr,"\n");
    //fprintf(stderr,"----------------------------------------------------------------------------\n");
    mpack_done_array(reader);

    for(i = ele_num; i > 0 && mpack_reader_error(reader) == mpack_ok; i--){
        tag = mpack_read_tag(reader);

        for(j = 0; j < tag.v.n; j++){
            child_tag = mpack_read_tag(reader);

            //unpack key
            child_tag = mpack_read_tag(reader);
            key = child_tag.v.u;

            #if 0
            key_used[key-1] = 1;
            c_depth = key;
            printf_int_key_val_by_index(c_depth,l_depth,reader,head[key-1]);
            l_depth = c_depth;
            #endif
            printf_int_key_val_by_index(key,reader,head[key-1]);
        }
        #if 0
        l_c_depth = c_depth;
        c_depth = 0;
        l_depth = 0;

        for(k = l_c_depth; k < list_num; k++){
            fprintf(stderr,"--    ");
        }
        #endif

        fprintf(stderr,"\n");
        mpack_done_array(reader);
    }
    mpack_done_array(reader);

    return rv;
}


static int process_show_cmds(char * content, int size)
{
	int rv = 0;
	int array_num = 0, list_num = 0, num = 0;
    int head_type = 0;
	mpack_reader_t reader;
    mpack_tag_t tag;
    int i = 0;

	mpack_reader_init(&reader,content,sizeof(content),size);

	array_num = mpack_expect_array(&reader);
    head_type = mpack_expect_u64(&reader);

    switch(head_type){
        case ONE_DIMENSIONAL_HEAD:
            show_simple_dimensional_without_head(&reader,array_num-1);
            break;
        case TWO_DIMENSIONAL_NO_HEAD:
            show_simple_dimensional_without_head(&reader,array_num-1);
            break;
        case TWO_DIMENSIONAL_WITH_HEAD:
            list_num = mpack_expect_array(&reader);
            show_simple_dimensional_with_print_head(&reader,list_num,array_num-2);
            break;
        case TWO_DIMENSIONAL_WITH_SPE_HEAD:
            list_num = mpack_expect_array(&reader);
            show_simple_dimensional_with_sparse_head(&reader,list_num,array_num-2);
            break;
        case MUL_DIMENSIONAL_WITH_HEAD:
		    for(i = array_num-1; i > 0 && mpack_reader_error(&reader) == mpack_ok; i--){
                num = mpack_expect_array(&reader);

                tag = mpack_read_tag(&reader);
                unpack_by_type(&tag,&reader,NULL,0);

                list_num = mpack_expect_array(&reader);
                show_mul_dimensional_with_not_print_head(&reader,list_num,num-2);

                mpack_done_array(&reader);
            }
            break;
        case MUL_DIMENSIONAL_NO_HEAD:
            show_mul_dimensional_without_head(&reader,array_num-1);
            break;
    }

	return rv;
}


int process_msg(string_buf *sb, unsigned long long slot_mask, int opcode)
{
	int rv = 0;
	char data_buf[MAX_CMD_LEN]={0};
	int size = 0, i = 0;
	req_t req_data;
	res_t *res[MAX_SLOT_NUM];
	res_t res0[MAX_SLOT_NUM];
	uint32_t nres = 0;

	if(sb->buf){
		//fprintf(stderr,"js_str:%s\n",sb->buf);
		json2msgpack(sb->buf,data_buf,&size);
		string_buf_free(sb);
	}

	nclt_req_fill(&req_data, size, opcode, slot_mask, data_buf);

	for(i = 0; i < MAX_SLOT_NUM; i++){
		memset(&res0[i], 0, sizeof(res_t));
        res[i] = &res0[i];
	}

	rv = nclt_sr_plus(&req_data, res, &nres, 10000);
	for(i = 0; i < nres; i++){
		if(res[i]->rv == 0){
			if(res[i]->content){
	//			fprintf(stderr,"slot %d:\n",res[i]->slot);
				rv = process_show_cmds((char *)res[i]->content, res[i]->size);
				if(rv != 0){
					fprintf(stderr,"slot %d %s\n",res[i]->slot,_oe_error(res[i]->rv));
				}
				free(res[i]->content);
			}else{
				fprintf(stderr,"set ok\n");
			}
		}else{
			fprintf(stderr,"%s\n",_oe_error(res[i]->rv));
			break;
		}
	}

	return rv;
}
