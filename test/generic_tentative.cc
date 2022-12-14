#include "label.hpp"
#include "generic_tentative.hpp"

int
main()
{
  generic_tentative<label_robe> T(1);
  // l1 <= l2, l1 || l2
  auto l1 = label_robe(label(1, {0, 5}), 0);
  auto l2 = label_robe(label(2, {0, 6}), 0);

  T.push(l1);
  assert(has_better_or_equal(T, l1));
  assert(!has_better_or_equal(T, l2));
}
