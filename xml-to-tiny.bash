#!/bin/bash

cat "$@" | sed -r \
	-e "s,<\?[^<>/]+\?>,#include \"nm-types.h\"\n,g" \
	-e "s,<\!--,/*,g" \
	-e "s,-->,*/,g" \
	-e "s,<([^<>/]+)>([^<>]+)</[^<>/]+>,\1 \2,g" \
	-e "s,<([^<>/]+)>,\1 {,g" \
	-e "s,</[^<>/]+>,},g" \
	-e "s,\<device_api_ver 1\>,api1,g" \
	-e "s,\<device_api_ver 2\>,api2,g" \
	-e "s,\<device_api_ver 3\>,api3,g" \
	-e "s,\<dir IN\>,in,g" \
	-e "s,\<dir OUT\>,out,g" \
	-e "s,\<data_type CTRL\>,ctrl,g" \
	-e "s,\<data_type ASYNC\>,async,g" \
	-e "s,\<data_type SYNC\>,sync,g" \
	-e "s,\<data_type ISOC\>,isoc,g" \
	-e "s,\<port_id MOST\>,most,g" \
	-e "s,\<port_id USB\>,usb,g" \
	-e "s,\<port_id MLB\>,mlb,g" \
	-e "s,\<channel_id (\S+)\>,id(\1),g" \
	-e "s,\<device_type (\S+)\>,dev(\1),g" \
	-e "s,\<blockwidth (\S+)\>,bw(\1),g" \
	-e "s,\<instance (\S+)\>,inst(\1),g" \
	-e "s,\<channel_addr_hex (\S+)\>,ca(0x\1),g" \
	-e "s,\<device_type_hex (\S+)\>,dev(0x\1),g" \
