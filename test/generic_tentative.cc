#include "generic_tentative.hpp"
#include "robed_label.hpp"

int
main()
{
  generic_tentative<robed_label> T(1);
  // l1 <= l2, l1 || l2
  auto l1 = robed_label(label(1, {0, 5}), 0);
  auto l2 = robed_label(label(2, {0, 6}), 0);

  T.push(l1);
  assert(has_better_or_equal(T, l1));
  assert(!has_better_or_equal(T, l2));
}
