/*******************************************************************************
 *
 *  Copyright (c) 2008 Cavium Networks 
 * 
 *  This file is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License, Version 2, as 
 *  published by the Free Software Foundation. 
 *
 *  This file is distributed in the hope that it will be useful, 
 *  but AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or 
 *  NONINFRINGEMENT.  See the GNU General Public License for more details. 
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this file; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA or 
 *  visit http://www.gnu.org/licenses/. 
 *
 *  This file may also be available under a different license from Cavium. 
 *  Contact Cavium Networks for more information
 *
 ******************************************************************************/

#ifndef VIRGO_H
#define VIRGO_H

// add by descent 2006/07/07
#define VIRGO
#ifdef VIRGO
// init phy or switch chip
#define INIT_PORT0_PHY star_gsw_config_ASIX();
#define INIT_PORT1_PHY star_gsw_config_AGERE();

// configure mac0/mac1 register
#define INIT_PORT0_MAC init_packet_forward(0);
#define INIT_PORT1_MAC init_packet_forward(1);

#define PORT0_LINK_DOWN disable_AN_VSC7385(0);
#define PORT0_LINK_UP disable_AN_VSC7385(1);

#define PORT1_LINK_DOWN AGERE_phy_power_down(1,1);
#define PORT1_LINK_UP AGERE_phy_power_down(1,0);

#define CREATE_NET_DEV0 star_gsw_probe(LAN_PORT);
#define CREATE_NET_DEV1 star_gsw_probe(WAN_PORT);
#define CREATE_NET_DEV2


#undef CONFIG_STR9100_VLAN_BASE
#undef CONFIG_HAVE_VLAN_TAG

#define CONFIG_STR9100_PORT_BASE
// for port base, port base max is 2 port.
// NET_DEV0 : rx->sp 0 (port 0)
#define NET_DEV0 STAR_GSW_LAN_DEV
// NET_DEV1 : rx->sp 1 (port 1)
#define NET_DEV1 STAR_GSW_WAN_DEV

// for star_gsw_send_packet
// port base and vlan base packet flow
#define PORT_BASE_PMAP_LAN_PORT PORT0
#define PORT_BASE_PMAP_WAN_PORT PORT1
#define PORT_BASE_PMAP_EWC_PORT INVALID_PORT_BASE_PMAP_PORT

#define MODEL "VIRGO"

// OPEN_PORT include 2 actions
// 1. enable mac port
// 2. link up port
#define OPEN_PORT(dev) \
{ \
        u32 mac_port_config; \
 \
	if (dev == STAR_GSW_LAN_DEV) { \
		memcpy(dev->dev_addr, star_gsw_info.vlan[LAN_GID].vlan_mac, 6);\
       		PRINT_INFO("open mac port 0\n");\
	        mac_port_config = GSW_MAC_PORT_0_CONFIG;\
       		/* enable port 0 */ \
	        mac_port_config &= (~(0x1 << 18));\
       		GSW_MAC_PORT_0_CONFIG = mac_port_config;\
		PORT0_LINK_UP\
	} \
	if(dev == STAR_GSW_WAN_DEV) { \
		memcpy(dev->dev_addr, star_gsw_info.vlan[WAN_GID].vlan_mac, 6);\
       		PRINT_INFO("open mac port 1\n");\
	        mac_port_config = GSW_MAC_PORT_1_CONFIG;\
       		/* enable port 0 */ \
	        mac_port_config &= (~(0x1 << 18));\
       		GSW_MAC_PORT_1_CONFIG = mac_port_config;\
		PORT1_LINK_UP\
	} \
}

// CLOSE_PORT include 2 actions
// 1. disable mac port
// 2. link down port
#define CLOSE_PORT(dev) \
{ \
        u32 mac_port_config; \
 \
	if (dev == STAR_GSW_LAN_DEV) { \
       		PRINT_INFO("close mac port 0\n");\
		PORT0_LINK_DOWN\
	        mac_port_config = GSW_MAC_PORT_0_CONFIG;\
       		/* disable port 0 */ \
	        mac_port_config |= ((0x1 << 18));\
       		GSW_MAC_PORT_0_CONFIG = mac_port_config;\
	} \
	if(dev == STAR_GSW_WAN_DEV) { \
       		PRINT_INFO("close mac port 1\n");\
		PORT1_LINK_DOWN\
	        mac_port_config = GSW_MAC_PORT_1_CONFIG;\
       		/* disable port 1 */ \
	        mac_port_config |= ((0x1 << 18));\
       		GSW_MAC_PORT_1_CONFIG = mac_port_config;\
	} \
}


#define VLAN0_VID			(0x111)
#define VLAN1_VID			(0x222)
#define VLAN2_VID			(0x333)
#define VLAN3_VID			(0x444)
#define VLAN4_VID			(0x555)
#define VLAN5_VID			(0x666)
#define VLAN6_VID			(0x777)
#define VLAN7_VID			(0x888)


#define VLAN0_GROUP			(PORT0 | PORT1 | CPU_PORT)
#define VLAN1_GROUP			(PORT0 | CPU_PORT)
#define VLAN2_GROUP			(PORT1 | CPU_PORT)
#define VLAN3_GROUP			(0)
#define VLAN4_GROUP			(0)
#define VLAN5_GROUP			(0)
#define VLAN6_GROUP			(0)
#define VLAN7_GROUP			(0)

#define VLAN0_VLAN_TAG			(0)
#define VLAN1_VLAN_TAG			(0)
#define VLAN2_VLAN_TAG			(0)
#define VLAN3_VLAN_TAG			(0)
#define VLAN4_VLAN_TAG			(0)
#define VLAN5_VLAN_TAG			(0)
#define VLAN6_VLAN_TAG			(0)
#define VLAN7_VLAN_TAG			(0)

//#define PORT0_PVID			(VLAN0_GROUP_ID)
//#define PORT1_PVID			(VLAN2_GROUP_ID)
//#define CPU_PORT_PVID			(VLAN1_GROUP_ID)

static u8 my_vlan0_mac[6] = {0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x11};
static u8 my_vlan1_mac[6] = {0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x21};
static u8 my_vlan2_mac[6] = {0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x31};
static u8 my_vlan3_mac[6] = {0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0x41};

// this value is for hnat
// GID is vlan group id
#define LAN_GID 1
#define WAN_GID 2



#endif // VIRGO

#endif

