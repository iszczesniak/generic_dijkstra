#include "helpers.hpp"

#include "generic_label.hpp"
#include "units.hpp"

#include <list>
#include <set>

using label = generic_label<double, CU>;

using namespace std;

// *****************************************************************
// Helper functions.
// *****************************************************************

bool
is_comparable(const label &li, const label &lj)
{
  // Either li \preceq lj or lj \preceq li holds.
  return boe(li, lj) || boe(lj, li);
}

bool
is_incomparable(const label &li, const label &lj)
{
  // Neither li \preceq lj nor lj \preceq li holds.
  return !is_comparable(li, lj);
}

// Returns all RIs rj such that: rj subset ri
auto
sub_RIs(const CU &ri)
{
  set<CU> l;
  assert(!ri.empty());

  for(auto s = ri.size(); --s;)
    for(auto i = ri.min(); i + s <= ri.max(); ++i)
      l.insert(CU(i, i + s));

  return l;
}

void
test_sub_RIs()
{
  assert(sub_RIs(CU{0, 1}).empty());

  auto l1 = sub_RIs(CU{0, 2});
  assert(l1.size() == 2);
  assert(l1.contains(CU(0, 1)));
  assert(l1.contains(CU(1, 2)));

  auto l2 = sub_RIs(CU{0, 3});
  assert(l2.size() == 5);
  assert(l2.contains(CU(0, 2)));
  assert(l2.contains(CU(1, 3)));
  assert(l2.contains(CU(0, 1)));
  assert(l2.contains(CU(1, 2)));
  assert(l2.contains(CU(2, 3)));
}

// Returns RIs rj such that: rj supset ri.
// rj must be included in omega.
auto
sup_RIs(const CU &ri, const CU &omega)
{
  set<CU> l;

  for(auto i = omega.min(); i <= ri.min(); ++i)
    for(auto j = ri.max(); j <= omega.max(); ++j)
      if (CU(i, j) != ri)
        l.insert(CU(i, j));

  return l;
}

void
test_sup_RIs()
{
  assert(sup_RIs(CU{0, 1}, CU(0, 1)).empty());
  assert(sup_RIs(CU{0, 10}, CU(0, 10)).empty());

  auto l1 = sup_RIs(CU{0, 1}, CU(0, 2));
  assert(l1.size() == 1);
  assert(l1.contains(CU(0, 2)));

  auto l2 = sup_RIs(CU{1, 2}, CU(0, 2));
  assert(l2.size() == 1);
  assert(l2.contains(CU(0, 2)));

  auto l3 = sup_RIs(CU{1, 3}, CU(0, 4));
  assert(l3.size() == 3);
  assert(l3.contains(CU(0, 3)));
  assert(l3.contains(CU(1, 4)));
  assert(l3.contains(CU(0, 4)));
}

// Returns RIs rj such that ri || rj, rj is on the left of ri.
auto
icl_RIs(const CU &ri, const CU &omega)
{
  assert(includes(omega, ri));

  set<CU> l;

  for(auto i = omega.min(); i < ri.min(); ++i)
    for(auto j = i + 1; j < ri.max(); ++j)
      l.insert(CU(i, j));

  return l;
}

// Returns RIs rj such that ri || rj, rj is on the right of ri.
auto
icr_RIs(const CU &ri, const CU &omega)
{
  assert(includes(omega, ri));

  set<CU> l;

  for(auto i = ri.min() + 1; i < omega.max(); ++i)
    for(auto j = std::max(i + 1, ri.max() + 1);
        j < omega.max(); ++j)
      l.insert(CU(i, j));

  return l;
}

// Returns RIs rj such that: ri || rj
auto
incomparable_RIs(const CU &ri, const CU &omega)
{
  auto l1 = icl_RIs(ri, omega);
  auto l2 = icr_RIs(ri, omega);

  l1.insert(l2.begin(), l2.end());

  return l1;
}

void
incomparable_RIs()
{
}

// *****************************************************************
// Test all relations.
// *****************************************************************

void
test_relations()
{
  CU omega(0, 30);
  // This label could be any.
  label li(10, {10, 20});

  // -----------------------------------------------------------------
  // Column 1.
  for(auto &rj: sub_RIs(get_resources(li)))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      assert(is_less(li, lj1));

      // Row 2.
      label lj2(get_cost(li), rj);
      assert(is_less(li, lj2));

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      assert(is_greater(li, lj3));
    }

  // Column 2.
  {
    // Row 1.
    label lj1(get_cost(li) + 1, get_resources(li));
    assert(is_less(li, lj1));

    // Row 2.
    label lj2(get_cost(li), get_resources(li));
    assert(is_equal(li, lj2));

    // Row 3.
    label lj3(get_cost(li) - 1, get_resources(li));
    assert(is_greater(li, lj3));
  }

  // Column 3.
  for(auto &rj: sup_RIs(get_resources(li), omega))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      assert(is_less(li, lj1));

      // Row 2.
      label lj2(get_cost(li), rj);
      assert(is_greater(li, lj2));

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      assert(is_greater(li, lj3));
    }

  // Column 4.
  for(auto &rj: incomparable_RIs(get_resources(li), omega))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      assert(is_less(li, lj1));

      // Row 2.
      label lj2(get_cost(li), rj);
      if (get_resources(li) < rj)
        assert(is_less(li, lj2));
      else
        assert(is_greater(li, lj2));

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      assert(is_greater(li, lj3));
    }
}

// *****************************************************************
// Test transitivity of <.
// *****************************************************************

set<label>
worse_labels(const label &li, const CU &omega)
{
  set<label> s;

  // -----------------------------------------------------------------
  // Column 1.
  for(auto &rj: sub_RIs(get_resources(li)))
    {
      // Row 1.
      s.emplace(get_cost(li) + 1, rj);
      // Row 2.
      s.emplace(get_cost(li), rj);
    }

  // Column 2. Row 1.
  s.emplace(get_cost(li) + 1, get_resources(li));

  // Column 3. Row 1.
  for(auto &rj: sup_RIs(get_resources(li), omega))
    s.emplace(get_cost(li) + 1, rj);

  // Column 4.
  for(auto &rj: incomparable_RIs(get_resources(li), omega))
    {
      // Row 1.
      s.emplace(get_cost(li) + 1, rj);

      // Row 2.
      if (get_resources(li) < rj)
        s.emplace(get_cost(li), rj);
    }

  return s;
}

void
test_transitivity()
{
  CU omega(0, 30);

  // This label could be any, but with at least three units.
  label li(10, {10, 20});

  for(const auto &lj: worse_labels(li, omega))
    for(const auto &lk: worse_labels(lj, omega))
      {
        assert(is_less(li, lj));
        assert(is_less(lj, lk));
        assert(is_less(li, lk));
      }
}

// Test the better-or-equal incomparability to pinpoint the reason why
// this relation is intransitive.
//
// The relation is intransitive since:
//
// * labels are of different cost, but it is intransitive for labels of
// equal cost and incomparable RIs.

auto
incomparable_labels(const label &li, const CU &omega)
{
  // slap - int-label pair
  using ilap = pair<int, label>;

  // slip - string-list pair
  using slip = pair<string, list<ilap>>;

  list<slip> l;

  // A.  Column 1.  Row 3.
  l.push_back(slip("A", {}));
  for(int i = 1; auto &rj: sub_RIs(get_resources(li)))
    l.back().second.push_back(ilap(i++, {get_cost(li) - 1, rj}));

  // B.  Column 3.  Row 1.
  l.push_back(slip("B", {}));
  for(int i = 1; auto &rj: sup_RIs(get_resources(li), omega))
    l.back().second.push_back(ilap(i++, {get_cost(li) + 1, rj}));

  // C.  Column 4.  Row 1.
  l.push_back(slip("C", {}));
  for(int i = 1; auto &rj:
        incomparable_RIs(get_resources(li), omega))
    l.back().second.push_back(ilap(i++, {get_cost(li) + 1, rj}));

  // D.  Column 4.  Row 2.
  l.push_back(slip("D", {}));
  for(int i = 1; auto &rj:
        incomparable_RIs(get_resources(li), omega))
    l.back().second.push_back(ilap(i++, {get_cost(li), rj}));

  // E.  Column 4.  Row 3.
  l.push_back(slip("E", {}));
  for(int i = 1; auto &rj:
        incomparable_RIs(get_resources(li), omega))
    l.back().second.push_back(ilap(i++, {get_cost(li) - 1, rj}));

  return l;
}

void
test_intran_boe_incomp()
{
  CU omega(0, 30);
  label li(10, {10, 20});

  // Iterate over the table cells.
  for (const auto &[text_j, list_j]:
         incomparable_labels(li, omega))
    // Iterate over the (int-label) pairs of a table cell.
    for (const auto &[num_j, lj]: list_j)
      // Iterate over the table cells.
      for (const auto &[text_k, list_k]:
             incomparable_labels(lj, omega))
        // Iterate over the labels of a table cell.
        for (const auto &[num_k, lk]: list_k)
          {
            assert(is_incomparable(li, lj));
            assert(is_incomparable(lj, lk));
          }
}

int
main()
{
  test_sub_RIs();
  test_sup_RIs();
  test_incomparable_RIs();
  
  test_relations();
  test_transitivity();
  test_intran_boe_incomp();
}
