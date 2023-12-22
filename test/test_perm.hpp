#ifndef TEST_PERM_HPP
#define TEST_PERM_HPP

#include "generic_permanent2.hpp"
#include "robed_label.hpp"

template <template<typename...> typename C>
void
test_perm()
{
  std::vector<label> ls;
  ls.push_back({1, {0, 2}});
  ls.push_back({4, {0, 3}});
  ls.push_back({6, {0, 4}});
  ls.push_back({2, {1, 3}});
  ls.push_back({5, {1, 4}});
  ls.push_back({3, {2, 4}});

  // We produce 720 permutations of the same 6 labels defined above.
  // These six labels are incomparable, so has_better_or_equal should
  // always return false, and insertion should always be successfull.
  // The order in which we push labels should not be important, and
  // that's what we test with permutations.
  do
    {
      generic_permanent2<robed_label> P(1);

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
          assert(P[0].contains(cl));
        }
    } while(std::next_permutation(ls.begin(), ls.end()));
}

#endif // TEST_PERM_HPP
