#include "generic_permanent2.hpp"
#include "label_robe.hpp"
#include "test_perm.hpp"

void
boe()
{
  // generic_permanent2<label<SU>> P(1);

  // generic_label<int, SU> l1(1, {{0, 1}, {2, 3}});
  // generic_label<int, SU> l2(2, {{0, 1}});

  // // label(weight = 1, resources = {{0, 1}, {2, 3}})
  // P.push(robed_label(l1, 0));
  // // label(weight = 2, resources = {{0, 1}});
  // P.push(robed_label(l2, 0));
}

int
main()
{
  boe();
  test_perm<generic_permanent2>();
}
