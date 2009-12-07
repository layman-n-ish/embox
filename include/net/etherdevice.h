/**
 * @file etherdevice.h
 *
 * @brief Definitions for the Ethernet handlers.
 * @date 5.03.2009
 * @author Anton Bondarev
 */
#ifndef ETHERDEVICE_H_
#define ETHERDEVICE_H_

#define NET_TYPE_ALL_PROTOCOL 0

#include <net/netdevice.h>
#include <net/if_ether.h>

typedef void (*ETH_LISTEN_CALLBACK)(void * pack);

/**
 * Determine if the Ethernet address is a multicast.
 * @param addr Pointer to a six-byte array containing the Ethernet address
 * @return true if the address is a multicast address.
 * By definition the broadcast address is also a multicast address.
 */
static inline int is_multicast_ether_addr(const uint8_t *addr) {
        return (0x01 & addr[0]);
}

/**
 * Determine if the Ethernet address is broadcast
 * @param addr Pointer to a six-byte array containing the Ethernet address
 * @return true if the address is the broadcast address.
 */
static inline int is_broadcast_ether_addr(const uint8_t *addr) {
        return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xff;
}

/**
 * Determine if give Ethernet address is all zeros.
 * @param addr Pointer to a six-byte array containing the Ethernet address
 * @return true if the address is all zeroes.
 */
static inline int is_zero_ether_addr(const uint8_t *addr) {
        return !(addr[0] | addr[1] | addr[2] | addr[3] | addr[4] | addr[5]);
}

/**
 * Determine if the given Ethernet address is valid
 * @param addr Pointer to a six-byte array containing the Ethernet address
 *
 * Check that the Ethernet address (MAC) is not 00:00:00:00:00:00, is not
 * a multicast address, and is not FF:FF:FF:FF:FF:FF.
 *
 * @return true if the address is valid.
 */
static inline int is_valid_ether_addr(const uint8_t *addr) {
        /* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
         * explicitly check for it here. */
         return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}

/**
 * Functions provided by eth.c
 */

/**
 * Extract hardware address from packet.
 * @param pack packet to extract header from
 * @param haddr destination buffer
 */
int eth_header_parse(const sk_buff_t *pack, unsigned char *haddr);

/**
 * Set new Ethernet hardware address.
 * @param dev network device
 * @param p socket address
 */
int eth_mac_addr(struct net_device *dev, void *p);

/**
 * Create the Ethernet header
 * @param pack buffer to alter
 * @param dev source device
 * @param type Ethernet type field
 * @param daddr destination address (NULL leave destination address)
 * @param saddr source address (NULL use device source address)
 * @paramlen packet length (<= pack->len)
 */
extern int eth_header(struct sk_buff *pack, struct net_device *dev,
                    unsigned short type, void *daddr, void *saddr, unsigned len);

/**
 * Rebuild the Ethernet MAC header.
 * @param pack socket buffer to update
 */
extern int eth_rebuild_header(struct sk_buff *pack);

/**
 * Setup Ethernet network device
 * @param dev network device
 * Fill in the fields of the device structure with Ethernet-generic values.
 */
extern void ether_setup(net_device_t *dev);

/**
 * Allocates and sets up an Ethernet device
 */
extern net_device_t *alloc_etherdev();

#endif /* ETHERDEVICE_H_ */
