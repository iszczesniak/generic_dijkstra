#include "generic_permanent.hpp"
#include "label_robe.hpp"
#include "units.hpp"

// Make sure that has_better_or_equal works correctly.  We do not need
// to check much, because we rely on the definition of < (if j < i,
// then i cannot be better than or equal to j, because it can only be
// incomparable or worse), and the transitivity of < (i.e., j < k hold
// true for all labels k that follow label i, because i < k).  The
// definition and transitivity of < are completely tested in
// generic_label.cc.

using namespace std;

using robed_label = label_robe<CU>;
using label = robed_label::label_type;
  
int
main()
{
  generic_permanent<robed_label> P(1);

  // We're inserting labels with non-decreasing cost.
  robed_label rl1(label(1, {2, 4}), 0);
  assert(!has_better_or_equal(P, rl1));
  P.push(rl1);

  robed_label rl2(label(2, {0, 3}), 0);
  assert(!has_better_or_equal(P, rl2));
  P.push(rl2);

  robed_label rl3(label(3, {1, 3}), 0);
  assert(has_better_or_equal(P, rl3));

  robed_label rl4(label(4, {1, 4}), 0);
  assert(!has_better_or_equal(P, rl4));
  P.push(rl4);
  assert(has_better_or_equal(P, rl4));
}
