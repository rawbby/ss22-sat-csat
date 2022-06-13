#include "csat/csat.hpp"
#include "csat/io.h"

int
main()
{
  using namespace csat;

  const auto a = Lit(1);
  const auto b = Lit(2);
  const auto c = Lit(3);
  const auto d = Lit(4);

  const auto ab = Or({ Not(a), b });
  const auto c_ab = And({ c, ab });

  const auto phi = Not(c_ab);

  // internal print:
  PrintLn(a);
  PrintLn(ab);
  PrintLn(c_ab);
  PrintLn(phi);

  // same as std print:
  std::cout << a << std::endl;
  std::cout << ab << std::endl;
  std::cout << c_ab << std::endl;
  std::cout << phi << std::endl;

  return 0;
}
