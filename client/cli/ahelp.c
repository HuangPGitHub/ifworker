#include "my_parser.h"

char *helplist[MAX_CMD_NUM][MAX_CMD_WORD_NUM] = {
	{"1", "exit"},
	{"2", "show", "system"},
	{"3", "show", "board", "info"},
	{"4", "show", "board", "<slot-id>", "info"},
	{"3", "show", "board", "status"},
	{"2", "show", "configuration"},
	{"2", "save", "configuration"},
	{"2", "clear", "configuration"},
	{"4", "restore", "configuration", "default", "[quiet]"},
	{"3", "reset", "system", "[quiet]"},
	{"8", "import", "configuration", "ftp", "<ip_address>", "\"<account_name>\"", "\"<passwd>\"", "\"<file_name>\"", "\"[path]\""},
	{"8", "export", "configuration", "ftp", "<ip_address>", "\"<account_name>\"", "\"<passwd>\"", "\"<file_name>\"", "\"[path]\""},
	{"4", "set", "platform", "<slot-id>", "s40"},
	{"4", "set", "platform", "<slot-id>", "s320"},
	{"2", "sysname", "\"<sysname>\""},
	{"3", "datetime", "<slot-id>", "\"[datetime]\""},
	{"3", "timezone", "<slot-id>", "\"[timezone]\""},
	{"3", "reset", "board", "<slot-id>"},
	{"1", "exitshell"},
	{"3", "ipconfig", "<ip>", "<mask>"},
	{"2", "gateway", "<ip>"},
	{"5", "set", "slot", "<slot-id>", "opmode", "fr_16xe_fb_11qxe_2dxe"},
	{"5", "set", "slot", "<slot-id>", "opmode", "fr_20xe_fb_9qxe_4dxe"},
	{"5", "set", "slot", "<slot-id>", "opmode", "fr_24xe_fb_8qxe_4dxe"},
	{"2", "show", "slot"},
	{"4", "set", "chassis", "id", "<id>"},
	{"4", "set", "chassis", "mode", "dual_star"},
	{"4", "set", "chassis", "mode", "dual_dual_star"},
	{"8", "set", "slot", "<slot-id>", "ac1200", "backplane", "rxtx", "mode", "receive"},
	{"8", "set", "slot", "<slot-id>", "ac1200", "backplane", "rxtx", "mode", "transceiving"},
	{"5", "slot", "<slot-id>", "module", "<module-id>", "portmap"},
	{"4", "set", "system", "name", "<string>"},
	{"3", "show", "current", "application"},
	{"3", "show", "available", "application"},
	{"4", "add", "application", "<url>", "\"[local_path]\""},
	{"3", "delete", "application", "\"<filename>\""},
	{"4", "select", "application", "\"<filename>\"", "[sequence]"},
	{"4", "undo", "select", "application", "\"<filename>\""},
	{"3", "om", "list", "application"},
	{"4", "om", "add", "application", "<url>"},
	{"4", "om", "delete", "application", "\"<version>\""},
	{"6", "om", "select", "application", "\"<version>\"", "<slot_min>", "[slot_max]"},
	{"7", "om", "undo", "select", "application", "\"<version>\"", "<slot_min>", "[slot_max]"},
	{"5", "set", "interface", "<interface_list>", "autoneg", "on"},
	{"5", "set", "interface", "all", "autoneg", "on"},
	{"5", "set", "interface", "all", "autoneg", "off"},
	{"5", "set", "interface", "<interface_list>", "autoneg", "off"},
	{"5", "set", "interface", "<interface_list>", "pause", "on"},
	{"5", "set", "interface", "<interface_list>", "pause", "off"},
	{"4", "set", "interface", "<interface_list>", "shutdown"},
	{"4", "set", "interface", "<interface_list>", "no_shutdown"},
	{"6", "set", "interface", "<interface_list>", "force", "up", "enable"},
	{"6", "set", "interface", "<interface_list>", "force", "up", "disable"},
	{"5", "set", "interface", "<interface_list>", "duplex", "full"},
	{"5", "set", "interface", "<interface_list>", "duplex", "half"},
	{"5", "set", "interface", "<interface_list>", "speed", "<1000|10000>"},
	{"6", "set", "interface", "<interface_list>", "mac_address", "learning", "on"},
	{"6", "set", "interface", "<interface_list>", "mac_address", "learning", "off"},
	{"4", "show", "interface", "<interface_list>", "info"},
	{"4", "show", "interface", "all", "info"},
	{"4", "show", "interface", "<interface_list>", "status"},
	{"4", "show", "interface", "all", "status"},
	{"5", "show", "interface", "<interface_list>", "counter", "[verbose]"},
	{"5", "show", "interface", "all", "counter", "[verbose]"},
	{"4", "clear", "interface", "all", "counter"},
	{"4", "clear", "interface", "<interface_list>", "counter"},
//	{"5", "set", "interface", "<interface_list>", "mode", "loopback"},
	{"5", "set", "interface", "<interface_list>", "mode", "normal"},
	{"5", "set", "interface", "<interface_list>", "mode", "lpbkmacnear"},
	{"5", "set", "interface", "<interface_list>", "mode", "lpbkmacfar"},
	{"5", "set", "interface", "<interface_list>", "mode", "lpbkpcsnear"},
	{"5", "set", "interface", "<interface_list>", "mode", "lpbkserdesnear"},
	{"5", "set", "interface", "<interface_list>", "mode", "lpbkserdesfar"},
	{"8", "set", "interface", "<interface_list>", "connect", "to", "fpga", "{ab,", "a9}"},
	{"6", "set", "interface", "<interface_list>", "disconnect", "from", "fpga"},
	{"6", "set", "interface", "<interface_list>", "connect", "to", "ac1200"},
	{"6", "set", "interface", "<interface_list>", "disconnect", "to", "ac1200"},
	{"5", "set", "interface", "<interface_list>", "descript", "<string>"},
	{"4", "add", "inports", "<inports_id>", "<interface_list>"},
	{"4", "delete", "inports", "<inports_id>", "<interface_list>"},
	{"5", "add", "outports", "<outports_id>", "hash", "<interface_list>"},
	{"7", "add", "outports", "<outports_id>", "hash", "<interface_list>", "weight", "<weight>"},
	{"5", "add", "outports", "<outports_id>", "mc", "<interface_list>"},
	{"4", "delete", "outports", "<outports_id>", "<interface_list>"},
	{"10", "set", "outports", "<outports_id>", "load_balance", "{sip,", "dip,", "ip,", "smac,", "dmac,", "mac}"},
	{"10", "set", "outports", "global", "load_balance", "{sip,", "dip,", "ip,", "smac,", "dmac,", "mac}"},
	{"4", "set", "load_balance", "port", "enable"},
	{"4", "set", "load_balance", "port", "disable"},
	{"6", "set", "slot", "<slot-id>", "load_balance", "port", "enable"},
	{"6", "set", "slot", "<slot-id>", "load_balance", "port", "disable"},
	{"3", "show", "inports", "<inports_id>"},
	{"3", "show", "inports", "all"},
	{"3", "show", "outports", "<outports_id>"},
	{"3", "show", "outports", "all"},
	{"3", "set", "failover", "enable"},
	{"3", "set", "failover", "disable"},
	{"4", "set", "failover", "interval", "<interval>"},
	{"5", "set", "inports", "<inports_id>", "pvlan", "enable"},
	{"5", "set", "inports", "<inports_id>", "pvlan", "disable"},
	{"6", "set", "inports", "<inports_id>", "pvlan", "id", "<id>"},
	{"6", "set", "inports", "<inports_id>", "pvlan", "id", "auto"},
	{"4", "set", "inports", "rx_vlantag", "replace"},
	{"4", "set", "inports", "rx_vlantag", "reserve"},
	{"6", "set", "slot", "<slot-id>", "inports", "rx_vlantag", "replace"},
	{"6", "set", "slot", "<slot-id>", "inports", "rx_vlantag", "reserve"},
	{"6", "set", "outports", "<outports_id>", "vlan", "untag", "enable"},
	{"6", "set", "outports", "<outports_id>", "vlan", "untag", "disable"},
	{"8", "add", "mac", "static", "<mac_addr>", "inports", "<inports_id>", "outports", "<outports_id>"},
	{"6", "delete", "mac", "static", "<mac_addr>", "inports", "<inports_id>"},
	{"4", "delete", "mac", "static", "all"},
	{"3", "show", "mac", "static"},
	{"5", "show", "mac", "static", "<mac_addr>", "[verbose]"},
	{"6", "show", "mac", "static", "inports", "<inports_id>", "[verbose]"},
	{"6", "show", "mac", "static", "outports", "<outports_id>", "[verbose]"},
	{"3", "show", "mac", "dynamic"},
	{"4", "show", "mac", "dynamic", "<mac_addr>"},
	{"5", "show", "mac", "dynamic", "inports", "<inports_id>"},
	{"5", "show", "mac", "dynamic", "outports", "<outports_id>"},
	{"4", "delete", "mac", "dynamic", "<mac_addr>"},
	{"4", "delete", "mac", "dynamic", "all"},
	{"3", "mac", "aging_time", "<aging_time>"},
	{"3", "show", "mac", "aging_time"},
	{"2", "snmp", "start"},
	{"2", "snmp", "restart"},
	{"2", "snmp", "stop"},
	{"3", "snmp", "restore", "configuration"},
	{"2", "snmp", "status"},
	{"3", "snmp", "show", "configuration"},
	{"4", "snmp", "set", "sysname", "\"<value>\""},
	{"4", "snmp", "set", "syscontact", "\"<value>\""},
	{"4", "snmp", "set", "syslocation", "\"<value>\""},
	{"4", "snmp", "set", "rocommunity", "\"<value>\""},
	{"4", "snmp", "set", "rwcommunity", "\"<value>\""},
	{"5", "snmp", "set", "trap", "version", "\"<value>\""},
	{"4", "snmp", "set", "username", "\"<value>\""},
	{"5", "snmp", "set", "auth", "type", "md5"},
	{"5", "snmp", "set", "auth", "type", "sha"},
	{"5", "snmp", "set", "auth", "password", "\"<value>\""},
	{"5", "snmp", "set", "priv", "type", "des"},
	{"5", "snmp", "set", "priv", "type", "aes"},
	{"5", "snmp", "set", "priv", "password", "\"<value>\""},
	{"4", "snmp", "set", "access", "noauth"},
	{"4", "snmp", "set", "access", "auth"},
	{"4", "snmp", "set", "access", "priv"},
	{"5", "snmp", "add", "trap", "ip", "<ip_addr>"},
	{"5", "snmp", "delete", "trap", "ip", "<ip_addr>"},
	{"2", "help", "link_protect"},
	{"2", "help", "infoshow"},
	{"2", "help", "l2cmds"},
	{"2", "help", "interface"},
	{"2", "help", "rule"},
	{"2", "help", "trunk"},
	{"2", "help", "system"},
	{"2", "help", "configuration"},
	{"2", "help", "snmp"},
	{"2", "help", "inports"},
	{"2", "help", "outports"},
	{"3", "help", "set", "rule"},
	{"3", "help", "show", "rule"},
	{"3", "help", "set", "l2cmds"},
	{"3", "help", "show", "l2cmds"},
	{"3", "help", "set", "interface"},
	{"3", "help", "show", "interface"},
	{"3", "help", "clear", "interface"},
	{"3", "help", "add", "inports"},
	{"3", "help", "delete", "inports"},
	{"3", "help", "add", "outports"},
	{"3", "help", "delete", "outports"},
	{"3", "help", "set", "rule"},
	{"3", "help", "show", "rule"},
	{"3", "help", "snmp", "status"},
	{"3", "help", "snmp", "start"},
	{"3", "help", "snmp", "restart"},
	{"3", "help", "snmp", "stop"},
	{"4", "help", "snmp", "show", "configuration"},
	{"3", "help", "snmp", "set"},
	{"2", "help", "log"},
	{"3", "help", "log", "set"},
	{"3", "help", "log", "show"},
	{"3", "log", "enable", "{info|rule|error|warn|all}"},
	{"3", "log", "disable", "{info|rule|error|warn|all}"},
	{"4", "log", "host", "<host_ip>", "[port]"},
	{"5", "log", "no", "host", "<host_ip>", "[port]"},
	{"3", "log", "host", "local"},
	{"3", "show", "log", "[line]"},
	{"4", "show", "log", "info", "[line]"},
	{"4", "show", "log", "rule", "[line]"},
	{"4", "show", "log", "trace", "[line]"},
	{"4", "show", "log", "error", "[line]"},
	{"3", "show", "log", "<yyyy-mm-dd>"},
	{"6", "show", "log", "from", "<yyyy-mm-dd>", "to", "<yyyy-mm-dd>"},
	{"2", "reset", "log"},
	{"6", "set", "<ud_id>", "mode", "<ud_mode>", "offset", "<ud_offset>"},
	{"4", "add", "rule", "<rule_id>", "<class_entry>"},
	{"5", "delete", "rule", "<rule_id>", "entry", "<entry_id>"},
	{"5", "delete", "rule", "<rule_id>", "entry", "all"},
	{"4", "show", "rule", "<rule_id>", "action"},
	{"4", "show", "rule", "<rule_id>", "entry"},
	{"7", "set", "rule", "<rule_id>", "inports", "<inport_id>", "fw", "drop"},
	{"10", "set", "rule", "<rule_id>", "inports", "<inport_id>", "fw", "drop", "outervlan", "<min>", "<max>"},
	{"8", "set", "rule", "<rule_id>", "inports", "<inport_id>", "fw", "outports", "<outports_list>"},
	{"11", "set", "rule", "<rule_id>", "inports", "<inport_id>", "fw", "outports", "<outports_list>", "outervlan", "<min>", "<max>"},
	{"2", "soc", "install"},
	{"2", "soc", "uninstall"},
	{"2", "rule", "clear"},
	{"4", "cli_debug", "acl", "rule", "<id>"},
	{"4", "show", "rule", "<rule_id>", "counter"},
	{"6", "show", "rule", "<rule_id>", "entry", "<entry_id>", "counter"},
	{"4", "clear", "rule", "<rule_id>", "counter"},
	{"6", "clear", "rule", "<rule_id>", "entry", "<entry_id>", "counter"},
	{"3", "rcload", "<slot-id>", "<config_file>"},
	{"6", "set", "interface", "<smp>", "mirror", "to", "<smp>"},
	{"10", "set", "interface", "<smp>", "vlan_s", "<int>", "vlanmask", "<int>", "replace", "by", "<int>"},
	{"10", "set", "interface", "<smp>", "outervlan", "<int>", "vlanmask", "<int>", "replace", "by", "<int>"},
	{"3", "warm", "synconfig", "enable"},
	{"3", "warm", "synconfig", "disable"},
	{"3", "link_protect", "create", "<olp_id>"},
	{"4", "link_protect", "create", "<olp_id>", "<olp_property>"},
	{"3", "link_protect", "destroy", "<olp_id>"},
	{"5", "link_protect", "interface", "add", "<olp_id>", "<smp>"},
	{"5", "link_protect", "interface", "delete", "<olp_id>", "<smp>"},
	{"6", "link_protect", "interface", "type", "set", "<olp_id>", "<if_type>"},
	{"7", "link_protect", "interface", "type", "set", "<olp_id>", "<smp>", "<if_type>"},
	{"5", "link_protect", "monitor", "<id>", "link", "on"},
	{"5", "link_protect", "monitor", "<id>", "link", "off"},
	{"5", "link_protect", "monitor", "<id>", "fcs", "on"},
	{"5", "link_protect", "monitor", "<id>", "fcs", "off"},
	{"5", "link_protect", "monitor", "<id>", "traffic", "on"},
	{"5", "link_protect", "monitor", "<id>", "traffic", "off"},
	{"5", "link_protect", "monitor", "all", "<type>", "on"},
	{"5", "link_protect", "monitor", "all", "<type>", "off"},
	{"4", "link_protect", "status", "show", "<olp_id>"},
	{"6", "link_protect", "set", "inline", "min", "rx_rate", "<rate>"},
	{"6", "link_protect", "set", "inline", "min", "tx_rx_percent", "<percent>"},
	{"6", "link_protect", "set", "inline", "min", "rx_fcs_rate", "<rate>"},
	{"6", "link_protect", "set", "inline", "min", "tx_fcs_rate", "<rate>"},
	{"6", "link_protect", "set", "monitor", "min", "tx_rate", "<rate>"},
	{"6", "link_protect", "set", "monitor", "min", "rx_tx_percent", "<percent>"},
	{"6", "link_protect", "set", "monitor", "min", "rx_fcs_rate", "<rate>"},
	{"6", "link_protect", "set", "monitor", "min", "tx_fcs_rate", "<rate>"},
	{"6", "link_protect", "interface", "concatenate", "type", "<olp_id>", "loop"},
	{"6", "link_protect", "interface", "concatenate", "type", "<olp_id>", "cross"},
	{"5", "link_protect", "set", "heartbeat", "interval", "<value>"},
	{"5", "link_protect", "set", "heartbeat", "timeout", "<value>"},
	{"6", "link_protect", "slot", "<slot-id>", "create", "<olp_id>", "<olp_property>"},
	{"5", "link_protect", "slot", "<slot-id>", "destroy", "<olp_id>"},
	{"7", "link_protect", "slot", "<slot-id>", "interface", "add", "<olp_id>", "<smp>"},
	{"7", "link_protect", "slot", "<slot-id>", "interface", "delete", "<olp_id>", "<smp>"},
	{"8", "link_protect", "slot", "<slot-id>", "interface", "type", "set", "<olp_id>", "<if_type>"},
	{"9", "link_protect", "slot", "<slot-id>", "interface", "type", "set", "<olp_id>", "<smp>", "<if_type>"},
	{"7", "link_protect", "slot", "<slot-id>", "monitor", "<id>", "<type>", "on"},
	{"7", "link_protect", "slot", "<slot-id>", "monitor", "<id>", "<type>", "off"},
	{"7", "link_protect", "slot", "<slot-id>", "monitor", "all", "<type>", "on"},
	{"7", "link_protect", "slot", "<slot-id>", "monitor", "all", "<type>", "off"},
	{"6", "link_protect", "slot", "<slot-id>", "status", "show", "<olp_id>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "inline", "min", "rx_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "inline", "min", "tx_rx_percent", "<percent>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "inline", "min", "rx_fcs_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "inline", "min", "tx_fcs_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "monitor", "min", "tx_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "monitor", "min", "rx_tx_percent", "<percent>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "monitor", "min", "rx_fcs_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "set", "monitor", "min", "tx_fcs_rate", "<rate>"},
	{"8", "link_protect", "slot", "<slot-id>", "interface", "concatenate", "type", "<olp_id>", "loop"},
	{"8", "link_protect", "slot", "<slot-id>", "interface", "concatenate", "type", "<olp_id>", "cross"},
	{"7", "link_protect", "slot", "<slot-id>", "set", "heartbeat", "interval", "<value>"},
	{"7", "link_protect", "slot", "<slot-id>", "set", "heartbeat", "timeout", "<value>"},
	{"3", "link_protect", "show", "configuration"},
};

char *tokenlist[MAX_CMD_WORD_NUM][MAX_CMD_NUM] = {
	{"30", "exit", "show", "save", "clear", "restore", "reset", "import", "export", "set", "sysname", "datetime", "timezone", "exitshell", "ipconfig", "gateway", "slot", "add", "delete", "select", "undo", "om", "mac", "snmp", "help", "log", "soc", "rule", "cli_debug", "rcload", "warm"},
	{"43", "system", "board", "configuration", "platform", "slot", "chassis", "current", "available", "application", "select", "list", "add", "delete", "undo", "interface", "inports", "outports", "failover", "mac", "start", "restart", "stop", "restore", "status", "show", "set", "infoshow", "rule", "trunk", "snmp", "clear", "log", "enable", "disable", "host", "no", "install", "uninstall", "acl", "synconfig", "create", "destroy", "monitor"},
	{"50", "info", "status", "default", "ftp", "id", "mode", "module", "name", "application", "select", "global", "port", "all", "enable", "disable", "interval", "static", "dynamic", "configuration", "sysname", "syscontact", "syslocation", "rocommunity", "rwcommunity", "trap", "username", "auth", "priv", "access", "rule", "interface", "inports", "outports", "start", "restart", "stop", "show", "set", "host", "local", "trace", "error", "from", "add", "delete", "type", "inline", "monitor", "concatenate", "heartbeat"},
	{"48", "info", "opmode", "application", "autoneg", "pause", "shutdown", "force", "duplex", "speed", "status", "counter", "mode", "connect", "disconnect", "descript", "hash", "mc", "enable", "disable", "pvlan", "replace", "reserve", "inports", "vlan", "all", "outports", "version", "type", "password", "noauth", "auth", "priv", "ip", "configuration", "entry", "mirror", "outervlan", "set", "link", "fcs", "traffic", "min", "interval", "timeout", "create", "destroy", "interface", "monitor"},
	{"36", "backplane", "portmap", "on", "off", "up", "full", "half", "learning", "loopback", "normal","lpbkmacnear","lpbkmacfar","lpbkpcsnear","lpbkserdesnear","lpbkserdesfar", "to", "from", "port", "enable", "disable", "id", "untag", "inports", "sha", "des", "aes", "offset", "all", "add", "delete", "type", "show", "inline", "monitor", "concatenate", "heartbeat"},
	{"20", "rxtx", "enable", "disable", "on", "off", "fpga", "weight", "auto", "replace", "reserve", "fw", "counter", "vlanmask", "loop", "cross", "set", "min", "type", "interval", "timeout"},
	{"5", "mode", "outports", "drop", "on", "off"},
	{"6", "receive", "transceiving", "outervlan", "replace", "loop", "cross"},
	{"2", "outervlan", "by"},
	{"0"},
	{"0"},
};
