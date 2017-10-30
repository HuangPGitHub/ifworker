#ifndef __RCP_OLP_H__
#define __RCP_OLP_H__

#include "appdefs.h"

#if 0
int rpc_oe_olp_create(void *h, olp_id_t *id);
int rpc_oe_olp_destroy(void *h, olp_id_t *id);
int rpc_oe_olp_property_set(void *h, olp_id_t *id, olp_property_t *olp_property);
int rpc_oe_olp_interface_add(void *h, olp_id_t *id, smp_new_t *oe_smp);
int rpc_oe_olp_interface_del(void *h, olp_id_t *id, smp_new_t *oe_smp);
int rpc_oe_olp_interface_set_type(void *h, olp_id_t *id, smp_new_t *oe_smp, int type);
int rpc_oe_olp_work(void *h, olp_id_t *id, int on);
#endif
int rpc_oe_olp_show_status(olp_id_t *id, char **status);

#if 0
int rpc_oe_olp_set_value(void *h, int value, int opmode);
int rpc_oe_olp_interface_concatenate_type(void *h, olp_id_t *id, int type);
int rpc_oe_olp_set_heartbeat_interval(void *h, int interval);
int rpc_oe_olp_set_heartbeat_timeout(void *h, int timeout);
#endif

#endif
