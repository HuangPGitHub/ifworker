#ifndef __RPC_CLI_SYS_H__
#define __RPC_CLI_SYS_H__

#include "exp_def.h"
#include "rpc_common.h"

#if 0
int rpc_oe_get_config(void *h, int page_size,int page_index,char **config,int *page_total);
int rpc_oe_get_board_status(void *,
		int [SLOT_NUMBER_PER_SYSTEM],
		int [SLOT_NUMBER_PER_SYSTEM]);
int rpc_oe_get_board_status_snmp(void *,
		int [SLOT_NUMBER_PER_SYSTEM],
		int [SLOT_NUMBER_PER_SYSTEM]);
#endif
int rpc_oe_get_board_info(int, struct board_info_t *);
int rpc_oe_get_board_info_snmp(int, struct board_info_t *);
int rpc_oe_free_system(struct oe_system_t *system);
int rpc_oe_get_system(struct oe_system_t *);
int rpc_oe_get_system_snmp(struct oe_system_t *system);
int rpc_oe_save_or_get_config(int op, res_t **, int *);
#if 0
int rpc_oe_reset_system(void *);
int rpc_oe_restore_config(void *);
int rpc_oe_save_config(void *);
int rpc_oe_set_platform(void *, int, int);
int rpc_oe_import_config(void *, int, char *, char * , char *, char *);
int rpc_oe_export_config(void *, int, char *, char * , char *, char *);
int rpc_oe_get_logo(void *, char *);
#endif

int rpc_oe_get_info(int *, int *);

#if 0
int rpc_oe_set_sysname(void *, char *);
int rpc_oe_set_datetime(void *, char *);
int rpc_oe_get_datetime(void *, char *);
int rpc_oe_set_timezone(void *, char *);
int rpc_oe_reset_board(void *, int);
int rpc_oe_ipconfig(void *, int, int);
int rpc_oe_gateway(void *, int);
int rpc_oe_opmode_set(void *h, int mode);
int rpc_oe_rcload_bcm_config(void *h, int slot, char *filename);

int rpc_oe_get_used_info(void *h, rpc_used_info_t *used_info);
int rpc_oe_set_used_info(void *h, rpc_used_info_t *used_info);
#endif

#endif
