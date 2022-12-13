#include "generic_label.hpp"
#include "generic_permanent.hpp"
#include "units.hpp"

// Make sure that has_better_or_equal works correctly.  We do not need
// to check much, because we rely on the definition of < (if j < i,
// then i cannot be better than or equal to j, because it can only be
// incomparable or worse), and the transitivity of < (i.e., j < k hold
// true for all labels k that follow label i, because i < k).  The
// definition and transitivity of < are completely tested in
// generic_label.cc.

using label = generic_label<double, CU>;

struct label_robe: generic_label<double, CU>, index<unsigned>
{
  using label_type = generic_label<double, CU>;

  label_robe(const label_type &l, unsigned i):
    label_type(l), index<unsigned>(i)
  {
  }

  using label_type::operator<=>;
};
               
using namespace std;

int
main()
{
  generic_permanent<label_robe> P(1);
  // l1 <= l2, l1 || l2
  auto l1 = label_robe(label(1, {0, 5}), 0);
  auto l2 = label_robe(label(2, {0, 6}), 0);

  P.push(l1);
  assert(has_better_or_equal(P, l1));
  assert(!has_better_or_equal(P, l2));
}
