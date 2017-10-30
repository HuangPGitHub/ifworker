cmake_minimum_required(VERSION 2.8)
project(ngn_worker)

add_subdirectory(common)

add_subdirectory(debug_cli)
add_subdirectory(msgpk_tools)
add_subdirectory(client)

add_subdirectory(data_trans)
add_subdirectory(srvcore)
add_subdirectory(srvapi)
add_subdirectory(worker)

add_subdirectory(ew_snmp)
add_subdirectory(version)
add_subdirectory(tools)


