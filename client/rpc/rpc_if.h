#ifndef __RPC_MODULE_H__
#define __RPC_MODULE_H__

#include "def.h"
#include "rpc_def.h"
#include "rpc_common.h"

#if 0
int rpc_oe_set_port_autoneg(void *, smp_new_t *, int);
int rpc_oe_set_port_pause(void *, smp_new_t *, int);
int rpc_oe_set_port_shutdown(void *, smp_new_t *, int);
int rpc_oe_set_port_single_fiber_transmit(void *, smp_new_t *, int);
int rpc_oe_set_port_duplex(void *, smp_new_t *, int);
int rpc_oe_set_port_speed(void *, smp_new_t *, int);
int rpc_oe_set_port_mac_address_learning(void *, smp_new_t *, int);
int rpc_oe_get_interface_info(void *, smp_new_t *, struct oe_info_t *);
int rpc_oe_get_interface_info_snmp(void *, smp_new_t *, struct oe_info_t *);
int rpc_oe_get_interface_status(void *, smp_new_t *, struct oe_status_t *);
int rpc_oe_get_interface_counter(void *, smp_new_t *, int, struct oe_counter_t *);
#endif

int rpc_oe_get_interface_counter_snmp(smp_new_t *, int, struct oe_counter_t *);
int rpc_oe_get_interface_status_snmp(smp_new_t *, struct oe_status_t *);

int rpc_oe_clear_interface_counter(smp_new_t *);
int rpc_oe_get_snmp_trap_info(int, int, struct oe_snmp_trap_info_t *);
void rpc_oe_free_interface_status(struct oe_status_t *);
void rpc_oe_free_interface_counter(struct oe_counter_t *);
void rpc_oe_free_interface_info(struct oe_info_t *oe_info);
int rpc_oe_set_port_mode(smp_new_t *, int, int);
#if 0
int rpc_oe_get_interface_config(void *h,  smp_new_t *smp, struct oe_config_t *oe_config);
#endif

#endif

