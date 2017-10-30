#ifndef __RPC_SNMP_H__
#define __RPC_SNMP_H__

#include "def.h"
#include "rpc_common.h"

//extern int rpc_oe_snmp_option(void *, int);
//extern int rpc_oe_show_snmp_status(void *, char **);
//extern int rpc_oe_show_snmp_conf(void *, struct oe_snmp_conf_t *);
//extern int rpc_oe_set_snmp_conf(void *, int, char *);
extern void rpc_oe_free_snmp(struct oe_snmp_t *);
extern int rpc_oe_get_snmp_entry(smp_new_t *, struct oe_snmp_t *);
//extern void rpc_oe_free_snmp_conf(struct oe_snmp_conf_t *);
#endif
