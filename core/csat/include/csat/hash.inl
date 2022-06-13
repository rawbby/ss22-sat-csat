#ifndef CSAT_HASH_HPP
#define CSAT_HASH_HPP

#include "csat.hpp"

namespace csat::detail {

hash_t
HashLit(const op_ref_t& a)
{
  constexpr auto StdHashLit = std::hash<int32_t>();
  return static_cast<hash_t>(StdHashLit(CastLit(a)->value));
}

hash_t
HashAnd(const op_ref_t& a)
{
  return static_cast<hash_t>(reinterpret_cast<uintptr_t>(&a));
}

hash_t
HashOr(const op_ref_t& a)
{
  return static_cast<hash_t>(reinterpret_cast<uintptr_t>(&a));
}
}

#endif
