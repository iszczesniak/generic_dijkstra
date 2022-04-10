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
is_incomparable(const label &li, const label &lj)
{
  // Neither li \preceq lj nor lj \preceq li holds.
  return !boe(li, lj) && !boe(lj, li);
}

// Returns RIs rj such that: rj subset ri
auto
sub_RIs(const CU &ri)
{
  list<CU> l;

  // Row 1.  Column 2.
  l.push_back(CU(ri.min() + 1, ri.max()));
  assert(ri < l.back());

  // Row 1.  Column 3.
  l.push_back(CU(ri.min() + 1, ri.max() - 1));
  assert(ri < l.back());

  // Row 2.  Column 3.
  l.push_back(CU(ri.min(), ri.max() - 1));
  assert(ri < l.back());

  return l;
}

// Returns RIs rj such that: rj supset ri
auto
sup_RIs(const CU &ri)
{
  list<CU> l;

  // Row 2.  Column 1.
  l.push_back(CU(ri.min(), ri.max() + 1));
  assert(ri > l.back());

  // Row 3.  Column 1.
  l.push_back(CU(ri.min() - 1, ri.max() + 1));
  assert(ri > l.back());

  // Row 3.  Column 2.
  l.push_back(CU(ri.min() - 1, ri.max()));
  assert(ri > l.back());

  return l;
}

// Returns RIs rj such that ri || rj, rj is on the left of ri.
auto
icl_RIs(const CU &ri)
{
  list<CU> l;

  l.push_back(CU(ri.min() - 1, ri.max() - 1));
  l.push_back(CU(ri.min() - ri.size(), ri.min()));
  l.push_back(CU(ri.min() - ri.size() - 1, ri.min() - 1));

  return l;
}

// Returns RIs rj such that ri || rj, rj is on the right of ri.
auto
icr_RIs(const CU &ri)
{
  list<CU> l;

  l.push_back(CU(ri.min() + 1, ri.max() + 1));
  l.push_back(CU(ri.max(), ri.max() + ri.size()));
  l.push_back(CU(ri.max() + 1, ri.max() + ri.size() + 1));

  return l;
}

// Returns RIs rj such that: ri || rj
auto
incomparable_RIs(const CU &ri)
{
  list<CU> l1 = icl_RIs(ri);
  list<CU> l2 = icr_RIs(ri);

  l1.insert(l1.end(), l2.begin(), l2.end());

  return l1;
}

// *****************************************************************
// Test all relations.
// *****************************************************************

void
test_relations()
{
  // This label could be any.
  label li(10, {100, 120});

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
  for(auto &rj: sup_RIs(get_resources(li)))
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
  for(auto &rj: incomparable_RIs(get_resources(li)))
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
worse_labels(const label &li)
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
  for(auto &rj: sup_RIs(get_resources(li)))
    s.emplace(get_cost(li) + 1, rj);

  // Column 4.
  for(auto &rj: incomparable_RIs(get_resources(li)))
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
  // This label could be any, but with at least three units.
  label li(10, {100, 120});

  for(const auto &lj: worse_labels(li))
    for(const auto &lk: worse_labels(lj))
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

set<label>
incomparable_labels(const label &li)
{
  set<label> s;

  int count = 0;

  // Column 1. Row 3.
  for(auto &rj: sub_RIs(get_resources(li)))
    {
      s.emplace(get_cost(li) - 1, rj);
      ++count;
    }

  // Column 3. Row 1.
  for(auto &rj: sup_RIs(get_resources(li)))
    {
      s.emplace(get_cost(li) + 1, rj);
      ++count;
    }

  // Column 4.
  for(auto &rj: incomparable_RIs(get_resources(li)))
    {
      // Row 1.
      s.emplace(get_cost(li) + 1, rj);
      // Row 2.
      s.emplace(get_cost(li), rj);
      // Row 3.
      s.emplace(get_cost(li) - 1, rj);

      count += 3;
    }

  // The cases are disjoint.
  assert(s.size() == count);
  
  return s;
}

void
test_intran_boe_incomp()
{
  label li(10, {100, 120});

  for (const auto &lj: incomparable_labels(li))
    for (const auto &lk: incomparable_labels(lj))
      {
        is_incomparable(li, lj);
        is_incomparable(lj, lk);
      }
}

int
main()
{
  test_relations();
  test_transitivity();
  test_intran_boe_incomp();
}
