#include "generic_permanent.hpp"
#include "generic_permanent2.hpp"
#include "generic_tentative.hpp"
#include "label_robe.hpp"
#include "units.hpp"

// We produce 720 permutations of the same 6 labels defined below.
// These six labels are incomparable, so has_better_or_equal should
// always return false, and insertion should always be successfull.
// The order in which we push labels should not be important, and
// that's what we test with permutations.

template <template<typename...> typename C>
void
test_perm()
{
  using robed_label = label_robe<CU>;
  using label = robed_label::label_type;

  std::vector<label> ls;
  ls.push_back({1, {0, 2}});
  ls.push_back({4, {0, 3}});
  ls.push_back({6, {0, 4}});
  ls.push_back({2, {1, 3}});
  ls.push_back({5, {1, 4}});
  ls.push_back({3, {2, 4}});

  do
    {
      C<robed_label> P(1);

      // Insert the labels.
      for(const auto &l: ls)
        {
          // A candidate label.
          robed_label cl(l, 0);
          assert(!has_better_or_equal(P, cl));
          P.push(cl);
        }

      // Make sure the inserted labels are there.
      for(const auto &l: ls)
        {
          // A candidate label.
          robed_label cl(l, 0);
          auto i = find(P[0].begin(), P[0].end(), cl);
          assert(i != P[0].end());
        }
    } while(std::next_permutation(ls.begin(), ls.end()));
}

int
main()
{
  test_perm<generic_permanent>();
  test_perm<generic_permanent2>();
  test_perm<generic_tentative>();
}
