#ifndef _LIB_ZTIME_H_
#define _LIB_ZTIME_H_


// === ZTime error codes =========================================================

/** No free channels are available */
#define ZTIME_ERROR  -101

// === Public API ==============================================================

/** @brief  Sample
 *
 *  @param  paramOne       First param
 *
 *  @return                zero - ok; negative - ZTime error code or negative errno
 */
extern int ztime_sample(int paramOne);


#endif
