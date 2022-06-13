#ifndef CSAT_IO_H
#define CSAT_IO_H

#include "csat.hpp"
#include <iostream>

namespace csat::detail {

CSAT_GENERATE_DISPATCH(Print, void);

CSAT_GENERATE_DISPATCH_(PrintF, void, std::ostream& os)
{
  CSAT_GENERATE_LOOKUP(PrintF);
  return lookup(self, os);
}

void
PrintFLit(const detail::op_ref_t& a, std::ostream& os)
{
  os << detail::CastLit(a)->value;
}

void
PrintFAnd(const detail::op_ref_t& a, std::ostream& os)
{
  os << '(';
  auto glue = "";
  for (const auto& op : *(detail::CastAnd(a)->clause)) {
    os << glue;
    Print(op);
    glue = " and ";
  }
  os << ')';
}

void
PrintFOr(const detail::op_ref_t& a, std::ostream& os)
{
  os << '(';
  auto glue = "";
  for (const auto& op : *(detail::CastOr(a)->clause)) {
    os << glue;
    Print(op);
    glue = " or ";
  }
  os << ')';
}

void
PrintLit(const detail::op_ref_t& a)
{
  PrintFLit(a, std::cout);
}

void
PrintAnd(const detail::op_ref_t& a)
{
  PrintFAnd(a, std::cout);
}

void
PrintOr(const detail::op_ref_t& a)
{
  PrintFOr(a, std::cout);
}
}

namespace csat {

using detail::Print;
using detail::PrintF;

void
PrintLn(const detail::op_ref_t& a)
{
  Print(a);
  std::cout << '\n';
}

void
PrintFLn(const detail::op_ref_t& a, std::ostream& os)
{
  PrintF(a, os);
  std::cout << '\n';
}
}

std::ostream& operator<<(std::ostream& os, const csat::detail::op_ref_t& a)
{
  csat::PrintF(a, os);
  return os;
}

std::ostream& operator<<(std::ostream& os, const csat::detail::lit_ref_t& a)
{
  csat::detail::PrintFLit(a, os);
  return os;
}

std::ostream& operator<<(std::ostream& os, const csat::detail::and_ref_t& a)
{
  csat::detail::PrintFAnd(a, os);
  return os;
}

std::ostream& operator<<(std::ostream& os, const csat::detail::or_ref_t& a)
{
  csat::detail::PrintFOr(a, os);
  return os;
}

#endif
