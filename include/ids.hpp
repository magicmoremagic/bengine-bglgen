/*!! include('common/ids', { symbol_prefix = 'BGLGEN_', value_prefix = 'be:bglgen:' }, {

   {'SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_BACKGROUND', 'service:simple_thread_pool_executor:background'},
   {'SERVICE_SIMPLE_INLINE_EXECUTOR_FOREGROUND', 'service:simple_inline_executor:foreground'},

}) !! 40 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#pragma once
#ifndef BE_BGLGEN_IDS_HPP_
#define BE_BGLGEN_IDS_HPP_

#include <be/core/id.hpp>

#define BEIDN_BGLGEN_SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_BACKGROUND "be:bglgen:service:simple_thread_pool_executor:background"
#define  BEID_BGLGEN_SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_BACKGROUND 0xA38FE5678AA4FE8C
#define BEIDN_BGLGEN_SERVICE_SIMPLE_INLINE_EXECUTOR_FOREGROUND      "be:bglgen:service:simple_inline_executor:foreground"
#define  BEID_BGLGEN_SERVICE_SIMPLE_INLINE_EXECUTOR_FOREGROUND      0xD2315FED86B17041

#ifdef BE_ID_EXTERNS

namespace be {
namespace ids {

extern const Id bglgen_service_simple_thread_pool_executor_background;
extern const Id bglgen_service_simple_inline_executor_foreground;

} // be::ids
} // be

#endif
#endif
#ifdef BE_ID_STATICS

namespace be {
namespace ids {

const Id bglgen_service_simple_thread_pool_executor_background = Id(BEIDN_BGLGEN_SERVICE_SIMPLE_THREAD_POOL_EXECUTOR_BACKGROUND);
const Id bglgen_service_simple_inline_executor_foreground = Id(BEIDN_BGLGEN_SERVICE_SIMPLE_INLINE_EXECUTOR_FOREGROUND);

} // be::ids
} // be

#endif

/* ######################### END OF GENERATED CODE ######################### */
