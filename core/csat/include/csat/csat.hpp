#ifndef CSAT_CSAT_HPP
#define CSAT_CSAT_HPP

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <span>
#include <tuple>
#include <utility>
#include <vector>

namespace csat::detail {

//   _        ___     ___    _  __  _   _   ____
//  | |      / _ \   / _ \  | |/ / | | | | |  _ \
//  | |     | | | | | | | | | ' /  | | | | | |_) |
//  | |___  | |_| | | |_| | | . \  | |_| | |  __/
//  |_____|  \___/   \___/  |_|\_\  \___/  |_|

// The struct_type_t is used to emulate pure virtual
// methods but less expensive as we know all subtypes.
typedef uint8_t struct_type_t;
constexpr struct_type_t STRUCT_TYPE_LIT = 0b00000000;
constexpr struct_type_t STRUCT_TYPE_AND = 0b00000001;
constexpr struct_type_t STRUCT_TYPE_OR = 0b00000010;

// The next macros do the magic that allows us
// cheap method look ups for our types.

#define CSAT_GENERATE_DISPATCH(name, result)                                   \
  result name##Lit(const op_ref_t&);                                           \
  result name##And(const op_ref_t&);                                           \
  result name##Or(const op_ref_t&);                                            \
  result name(const op_ref_t& self)                                            \
  {                                                                            \
    constexpr decltype(name)* lookup_[3]{ name##Lit, name##And, name##Or };    \
    const auto lookup = lookup_[self->t];                                      \
    return lookup(self);                                                       \
  }

#define CSAT_GENERATE_DISPATCH_(name, result, ...)                             \
  result name##Lit(const op_ref_t&, __VA_ARGS__);                              \
  result name##And(const op_ref_t&, __VA_ARGS__);                              \
  result name##Or(const op_ref_t&, __VA_ARGS__);                               \
  result name(const op_ref_t& self, __VA_ARGS__)

#define CSAT_GENERATE_LOOKUP(name)                                             \
  constexpr decltype(name)* lookup_[3]{ name##Lit, name##And, name##Or };      \
  const auto lookup = lookup_[self->t]

//  ____       _      ____    _____     _____  __   __  ____    _____   ____
// | __ )     / \    / ___|  | ____|   |_   _| \ \ / / |  _ \  | ____| / ___|
// |  _ \    / _ \   \___ \  |  _|       | |    \ V /  | |_) | |  _|   \___ \
// | |_) |  / ___ \   ___) | | |___      | |     | |   |  __/  | |___   ___) |
// |____/  /_/   \_\ |____/  |_____|     |_|     |_|   |_|     |_____| |____/

// === Operator Type ===

struct op_t
{
  const struct_type_t t;
  explicit op_t(struct_type_t t)
    : t(t)
  {
  }
};
typedef std::shared_ptr<op_t> op_ref_t;

// === Literal Type ===

struct lit_t : public op_t
{
  const int32_t value;
  explicit lit_t(int32_t value)
    : op_t(STRUCT_TYPE_LIT)
    , value(value)
  {
  }
};
typedef std::shared_ptr<lit_t> lit_ref_t;

// === Out of order Hash declaration ===
// note: Hash is needed by the Operator Set which
//       is needed for the And Type and Or Type.

typedef size_t hash_t;

hash_t
Hash(const op_ref_t& self);

constexpr auto CmpOpRef = [](const op_ref_t& a, const op_ref_t& b) {
  return Hash(a) < Hash(b);
};
typedef std::set<op_ref_t, decltype(CmpOpRef)> op_ref_set_t;
typedef std::shared_ptr<op_ref_set_t> op_ref_set_ref_t;

// === And Type ===

struct and_t : public op_t
{
  op_ref_set_ref_t clause;
  explicit and_t(std::initializer_list<op_ref_t> clause)
    : op_t(STRUCT_TYPE_AND)
    , clause(std::make_shared<op_ref_set_t>(clause))
  {
  }
  explicit and_t(op_ref_set_ref_t clause)
    : op_t(STRUCT_TYPE_AND)
    , clause(std::move(clause))
  {
  }
};
typedef std::shared_ptr<and_t> and_ref_t;

// === Or Type ===

struct or_t : public op_t
{
  op_ref_set_ref_t clause;
  explicit or_t(std::initializer_list<op_ref_t> clause)
    : op_t(STRUCT_TYPE_OR)
    , clause(std::make_shared<op_ref_set_t>(clause))
  {
  }
  explicit or_t(op_ref_set_ref_t clause)
    : op_t(STRUCT_TYPE_OR)
    , clause(std::move(clause))
  {
  }
};
typedef std::shared_ptr<or_t> or_ref_t;

//  _____  __   __  ____    _____      ____   _   _   _____    ____   _  __
// |_   _| \ \ / / |  _ \  | ____|    / ___| | | | | | ____|  / ___| | |/ /
//   | |    \ V /  | |_) | |  _|     | |     | |_| | |  _|   | |     | ' /
//   | |     | |   |  __/  | |___    | |___  |  _  | | |___  | |___  | . \
//   |_|     |_|   |_|     |_____|    \____| |_| |_| |_____|  \____| |_|\_\

bool
IsLit(const op_ref_t& op)
{
  return op->t == STRUCT_TYPE_LIT;
}

bool
IsAnd(const op_ref_t& op)
{
  return op->t == STRUCT_TYPE_AND;
}

bool
IsOr(const op_ref_t& op)
{
  return op->t == STRUCT_TYPE_OR;
}

//  _____  __   __  ____    _____      ____      _      ____    _____
// |_   _| \ \ / / |  _ \  | ____|    / ___|    / \    / ___|  |_   _|
//   | |    \ V /  | |_) | |  _|     | |       / _ \   \___ \    | |
//   | |     | |   |  __/  | |___    | |___   / ___ \   ___) |   | |
//   |_|     |_|   |_|     |_____|    \____| /_/   \_\ |____/    |_|

lit_ref_t
CastLit(const op_ref_t& op)
{
  return std::static_pointer_cast<lit_t>(op);
}

and_ref_t
CastAnd(const op_ref_t& op)
{
  return std::static_pointer_cast<and_t>(op);
}

or_ref_t
CastOr(const op_ref_t& op)
{
  return std::static_pointer_cast<or_t>(op);
}

//  _____      _       ____   _____    ___    ____    ___   _____   ____
// |  ___|    / \     / ___| |_   _|  / _ \  |  _ \  |_ _| | ____| / ___|
// | |_      / _ \   | |       | |   | | | | | |_) |  | |  |  _|   \___ \
// |  _|    / ___ \  | |___    | |   | |_| | |  _ <   | |  | |___   ___) |
// |_|     /_/   \_\  \____|   |_|    \___/  |_| \_\ |___| |_____| |____/

lit_ref_t
Lit(int32_t value)
{
  return std::make_shared<lit_t>(value);
}

and_ref_t
And(std::initializer_list<op_ref_t> ops)
{
  return std::make_shared<and_t>(ops);
}

and_ref_t
And(const op_ref_set_ref_t& ops)
{
  return std::make_shared<and_t>(ops);
}

or_ref_t
Or(std::initializer_list<op_ref_t> ops)
{
  return std::make_shared<or_t>(ops);
}

or_ref_t
Or(const op_ref_set_ref_t& ops)
{
  return std::make_shared<or_t>(ops);
}
}

//  __  __   _____   _____   _   _    ___    ____    ____
// |  \/  | | ____| |_   _| | | | |  / _ \  |  _ \  / ___|
// | |\/| | |  _|     | |   | |_| | | | | | | | | | \___ \
// | |  | | | |___    | |   |  _  | | |_| | | |_| |  ___) |
// |_|  |_| |_____|   |_|   |_| |_|  \___/  |____/  |____/

namespace csat {
namespace detail {

CSAT_GENERATE_DISPATCH(Hash, hash_t);
CSAT_GENERATE_DISPATCH(Not, op_ref_t);
}

// Move the public Methods into the public
// namespace.

using detail::And;
using detail::Lit;
using detail::Or;

using detail::IsAnd;
using detail::IsLit;
using detail::IsOr;

using detail::CastAnd;
using detail::CastLit;
using detail::CastOr;

using detail::Hash;
using detail::Not;
}

#include "hash.inl"
#include "not.inl"

#endif
