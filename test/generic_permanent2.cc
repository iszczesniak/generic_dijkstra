#include "generic_permanent2.hpp"
#include "label_robe.hpp"
#include "units.hpp"

using robed_label = label_robe<CU>;
using label = robed_label::label_type;

void
boe()
{
  generic_permanent2<robed_label> P(1);

  label l1(1, {0, 2});
  label l2(2, {0, 1});

  P.push(robed_label(l1, 0));
  assert(has_better_or_equal(P, robed_label(l2, 0)));
}

int
main()
{
  boe();
}
