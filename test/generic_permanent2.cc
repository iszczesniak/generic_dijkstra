#include "generic_permanent2.hpp"
#include "label_robe.hpp"
#include "test_perm.hpp"

using robed_label = label_robe<SU>;
using label = robed_label::label_type;

void
boe()
{
  generic_permanent2<label> P(1);

  label l1(1, {{0, 1}, {2, 3}});
  label l2(2, {{0, 1}});

  P.push(robed_label(l1, 0));
  P.push(robed_label(l2, 0));
}

int
main()
{
  boe();
  test_perm<generic_permanent2>();
}
