#include "generic_label.hpp"
#include "units.hpp"

#include <set>

using label = generic_label<double, CU>;

using namespace std;

// *****************************************************************
// Helper function.
// *****************************************************************

// Make sure that li < lj.
void
better(const label &li, const label &lj)
{
  assert(li < lj);
  assert(li <= lj);
  assert(li != lj);
  assert(!(li == lj));
  assert(!(li > lj));
  assert(!(li >= lj));
}

// Make sure that li == lj.
void
equal(const label &li, const label &lj)
{
  assert(li == lj);
  assert(!(li != lj));
  assert(li <= lj);
  assert(li >= lj);
  assert(!(li < lj));
  assert(!(li > lj));
}

// Make sure that li > lj.
void
worse(const label &li, const label &lj)
{
  better(lj, li);
}

bool
incomparable(const label &li, const label &lj)
{
  // Neither li \preceq lj nor lj \preceq li holds.
  return !(boe(li, lj)) && !(boe(lj, li));
}

// Returns RIs rj such that: ri < rj
auto
worse_RIs(const CU &ri)
{
  list<CU> l;

  // Row 1.  Column 1.
  l.push_back(CU(ri.min() + 1, ri.max() + 1));
  assert(ri < l.back());

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

// Returns RIs rj such that: ri > rj
auto
better_RIs(const CU &ri)
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

  // Row 3.  Column 3.
  l.push_back(CU(ri.min() - 1, ri.max() - 1));
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
// Test all relations
// *****************************************************************

void
test_relations()
{
  // This label could be any.
  label li(10, {100, 120});

  // -----------------------------------------------------------------
  // Column 1.
  for(auto &rj: worse_RIs(get_resources(li)))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      better(li, lj1);

      // Row 2.
      label lj2(get_cost(li), rj);
      better(li, lj2);

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      worse(li, lj3);
    }

  // Column 2.
  {
    // Row 1.
    label lj1(get_cost(li) + 1, get_resources(li));
    better(li, lj1);

    // Row 2.
    label lj2(get_cost(li), get_resources(li));
    equal(li, lj2);

    // Row 3.
    label lj3(get_cost(li) - 1, get_resources(li));
    worse(li, lj3);
  }

  // Column 3.
  for(auto &rj: better_RIs(get_resources(li)))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      better(li, lj1);

      // Row 2.
      label lj2(get_cost(li), rj);
      worse(li, lj2);

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      worse(li, lj3);
    }

  // Column 4.
  for(auto &rj: incomparable_RIs(get_resources(li)))
    {
      // Row 1.
      label lj1(get_cost(li) + 1, rj);
      better(li, lj1);

      // Row 2.
      label lj2(get_cost(li), rj);
      if (get_resources(li) < rj)
        better(li, lj2);
      else
        worse(li, lj2);

      // Row 3.
      label lj3(get_cost(li) - 1, rj);
      worse(li, lj3);
    }
}

// *****************************************************************
// transitivity
// *****************************************************************

set<label>
worse(const label &li)
{
  set<label> s;

  // -----------------------------------------------------------------
  // Column 1.
  for(auto &rj: worse_RIs(get_resources(li)))
    {
      // Row 1.
      s.emplace(get_cost(li) + 1, rj);
      // Row 2.
      s.emplace(get_cost(li), rj);
    }

  // Column 2. Row 1.
  s.emplace(get_cost(li) + 1, get_resources(li));

  // Column 3. Row 1.
  for(auto &rj: better_RIs(get_resources(li)))
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

// *****************************************************************
// Test transitivity exchaustively.
// *****************************************************************

void
test_transitivity()
{
  // This label could be any, but with at least three units.
  label li(10, {100, 120});

  for(const auto &lj: worse(li))
    for(const auto &lk: worse(lj))
      {
        better(li, lj);
        better(lj, lk);
        better(li, lk);
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
  for(auto &rj: worse_RIs(get_resources(li)))
    {
      s.emplace(get_cost(li) - 1, rj);
      ++count;
    }

  // Column 3. Row 1.
  for(auto &rj: better_RIs(get_resources(li)))
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
        assert(incomparable(li, lj));
        assert(incomparable(lj, lk));
      }
}

int
main()
{
  test_relations();
  test_transitivity();
  test_intran_boe_incomp();
}
