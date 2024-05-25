#ifndef MFIGHT_DEFS_H__
#define MFIGHT_DEFS_H__ 1

#undef MFIGHT_DEBUG_EXCEPT

#ifdef DEBUG
#define MFIGHT_DEBUG_EXCEPT
#else
#define MFIGHT_DEBUG_EXCEPT noexcept
#endif

#include <cstdint>
#include <numeric>

namespace mfight {

using tick_t = std::uint32_t;

float qNaN() noexcept {
	return std::numeric_limits<float>::quiet_NaN();
}

} // namespace mfight

#endif