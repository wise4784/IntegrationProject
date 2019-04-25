/*
 * sys_flush_cache.h
 *
 *  Created on: 16 Mar 2016
 *      Author: stuhssn@gmail.com
 */

#ifndef INCLUDE_SYS_FLUSH_CACHE_H_
#define INCLUDE_SYS_FLUSH_CACHE_H_

/*
 * dcacheCleanRange
 * Flushes a defined region from the cache to RAM
 */
 void _dcacheCleanRange_(const uint8_t * startAddress, const uint8_t * endAddress);

#endif /* INCLUDE_SYS_FLUSH_CACHE_H_ */
