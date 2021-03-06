/*
 * Copyright 2019, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <libpmemobj++/experimental/array.hpp>
#include <libpmemobj++/experimental/string.hpp>
#include <libpmemobj++/p.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/pext.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj++/transaction.hpp>
#include <stdexcept>
#include <string>

namespace std
{
template <>
struct hash<pmem::obj::experimental::string> {
	std::size_t
	operator()(const pmem::obj::experimental::string &data)
	{
		std::string str(data.cbegin(), data.cend());
		return std::hash<std::string>{}(str);
	}
};
}

namespace examples
{

namespace ptl = pmem::obj::experimental;

using pmem::obj::delete_persistent;
using pmem::obj::make_persistent;

using pmem::obj::p;
using pmem::obj::persistent_ptr;
using pmem::obj::pool;
using pmem::obj::pool_base;
using pmem::obj::transaction;

/**
 * Key - type of the key
 * Value - type of the value stored in hashmap
 * N - Size of hashmap
 */
template <typename Key, typename Value, std::size_t N>
class kv {
private:
	using bucket_type = ptl::vector<std::pair<Key, std::size_t>>;
	using table_type = ptl::array<bucket_type, N>;

	table_type table;
	ptl::vector<Value> values;

public:
	using value_type = Value;

	kv() = default;

	Value &
	at(const Key &key)
	{
		auto index = std::hash<Key>{}(key) % N;

		for (const auto &e : table[index])
		{
			if (e.first == key)
				return values[e.second];
		}

		throw std::out_of_range("no entry in simplekv");
	}

	void
	insert(const Key &key, const Value &val)
	{
		auto index = std::hash<Key>{}(key) % N;

		for (std::size_t i = 0; i < table[index].size(); ++i)
		{
			if (table[index][i].first == key)
			{
				values[table[index][i].second] = val;
				return;
			}
		}

		values.emplace_back(val);
		table[index].emplace_back(key, values.size() - 1);
	}

	auto begin() -> decltype(values.begin())
	{
		return values.begin();
	}

	auto end() -> decltype(values.end())
	{
		return values.end();
	}
};

} /* namespace examples */
