#ifndef CSAT_NOT_HPP
#define CSAT_NOT_HPP

#include "csat.hpp"

namespace csat::detail {

op_ref_t
NotLit(const op_ref_t& a)
{
  return Lit(-CastLit(a)->value);
}

op_ref_t
NotAnd(const op_ref_t& a)
{
  const auto and_op = CastAnd(a);

  auto ops = and_op->clause;
  auto not_ops = std::make_shared<op_ref_set_t>();
  for (const auto& op : *ops) {
    not_ops->insert(Not(op));
  }

  return Or(not_ops);
}

op_ref_t
NotOr(const op_ref_t& a)
{
  const auto or_op = CastOr(a);

  auto ops = or_op->clause;
  auto not_ops = std::make_shared<op_ref_set_t>();
  for (const auto& op : *ops) {
    not_ops->insert(Not(op));
  }

  return And(not_ops);
}
}

#endif
