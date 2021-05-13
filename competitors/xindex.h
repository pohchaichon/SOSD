#pragma once

#include "base.h"
#include "helper.h"
#include "./xindex/XIndex-R/xindex.h"
#include "./xindex/XIndex-R/xindex_impl.h"

#include <dtl/thread.hpp>

#include <type_traits>
#include <utility>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <atomic>
#include <memory>
#include <random>
#include <string>
#include <vector>

template<class KeyType, int size_scale>
class XIndex : public Competitor {
public:

  uint64_t Build(const std::vector<KeyValue<KeyType>>& data) {
		std::vector<Key> keys;
		std::vector<uint64_t> vals;
    keys.reserve(data.size());
    vals.reserve(data.size());
    for (auto& itm: data) {
      uint64_t idx = itm.value;
      if (size_scale > 1 && idx % size_scale != 0)
				continue;
      keys.push_back(Key(itm.key));
			vals.push_back(itm.value);
    }

    data_size_ = data.size();

    return util::timing([&] {
			map_ = new xindex::XIndex<Key, uint64_t>(
					keys, vals, 1, 6
				);
    });
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
		uint64_t val;
    auto res = map_->get(Key(lookup_key), val, 0);

		if(!res) std::cout << "not found" << std::endl;

    const uint64_t error = size_scale - 1;

    const uint64_t start = val < error ? 0 : val - error;
    const uint64_t
        stop = val + 1 > data_size_ ? data_size_ : val + 1; // stop is exclusive (that's why +1)

    return (SearchBound){ start, stop };
  }

  std::string name() const {
    return "XIndex";
  }

  std::size_t size() const {
		// unsupported
    return 0; 
  }

  int variant() const { return size_scale; }

private:
	class Key;
  uint64_t data_size_ = 0;
  xindex::XIndex<Key, uint64_t> *map_;

	class Key {
		typedef std::array<double, 1> model_key_t;

	 public:
		static constexpr size_t model_key_size() { return 1; }
		static Key max() {
			static Key max_key(std::numeric_limits<uint64_t>::max());
			return max_key;
		}
		static Key min() {
			static Key min_key(std::numeric_limits<uint64_t>::min());
			return min_key;
		}

		Key() : key(0) {}
		Key(uint64_t key) : key(key) {}
		Key(const Key &other) { key = other.key; }
		Key &operator=(const Key &other) {
			key = other.key;
			return *this;
		}

		model_key_t to_model_key() const {
			model_key_t model_key;
			model_key[0] = key;
			return model_key;
		}

		friend bool operator<(const Key &l, const Key &r) { return l.key < r.key; }
		friend bool operator>(const Key &l, const Key &r) { return l.key > r.key; }
		friend bool operator>=(const Key &l, const Key &r) { return l.key >= r.key; }
		friend bool operator<=(const Key &l, const Key &r) { return l.key <= r.key; }
		friend bool operator==(const Key &l, const Key &r) { return l.key == r.key; }
		friend bool operator!=(const Key &l, const Key &r) { return l.key != r.key; }

		uint64_t key;
	} PACKED;
};
