#ifndef BITSET_H
#define BITSET_H

#include <stdlib.h>

// Basic, easy to serialize bitset implementation
template <size_t N>
class Bitset {
  private:
	char arr[(N + 7) / 8];

  public:
	Bitset(): arr() {}
	~Bitset() {}

	inline bool test(size_t idx) const { return ((arr[(idx>>3)] >> (idx&7)) & 1); }
	inline void set(size_t idx) { arr[(idx>>3)] |= (0x1 << (idx&7)); }
	inline void reset(size_t idx) { arr[(idx>>3)] &= ~(0x1 << (idx&7)); }
	inline int count() const {
		int cnt = 0;
		for (size_t i = 0; i < (N + 7) / 8; ++i)
			cnt += __builtin_popcount(arr[i]);
		return cnt;
	}
};

#endif /* BITSET_H */