#include "generic_label.hpp"
#include "units.hpp"

#include <set>

using label = generic_label<double, CU>;

using namespace std;

// *****************************************************************
// Helper function.
// *****************************************************************

list<CU>
better_RI(const CU &ri)
{
  list<CU> l;

  // For case 2.a.
  l.push_back(CU(ri.min(), ri.max() + 1));
  // For case 3.a.
  l.push_back(CU(ri.min() - 1, ri.max() - 1));
  // For case 3.b.
  l.push_back(CU(ri.min() - 1, ri.max()));
  // For case 3.c.
  l.push_back(CU(ri.min() - 1, ri.max() + 1));

  return l;
}

list<CU>
incomparable_RI(const CU &ri)
{
  list<CU> l;

  l.push_back(CU(ri.min() + 1, ri.max() + 1));
  l.push_back(CU(ri.min() - 1, ri.max() - 1));

  l.push_back(CU(ri.max(), ri.max() + ri.size()));
  l.push_back(CU(ri.min() - ri.size(), ri.min()));

  l.push_back(CU(ri.max() + 1, ri.max() + ri.size() + 1));
  l.push_back(CU(ri.min() - ri.size() - 1, ri.min() - 1));

  return l;
}

list<CU>
worse_RI(const CU &ri)
{
  list<CU> l;

  // For case 1.a.
  l.push_back(CU(ri.min() + 1, ri.max() + 1));
  // For case 1.b.
  l.push_back(CU(ri.min() + 1, ri.max()));
  // For case 1.c.
  l.push_back(CU(ri.min() + 1, ri.max() - 1));
  // For case 2.c.
  l.push_back(CU(ri.min(), ri.max() - 1));

  return l;
}

// Make sure that li < lj.
void
lt(const label &li, const label &lj)
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
eq(const label &li, const label &lj)
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
gt(const label &li, const label &lj)
{
  assert(li > lj);
  assert(li >= lj);
  assert(li != lj);
  assert(!(li == lj));
  assert(!(li < lj));
  assert(!(li <= lj));
}

// *****************************************************************
// Test all relations
// *****************************************************************

void
test_relations()
{
  // This label could be any.
  label li(10, {10, 20});

  // -----------------------------------------------------------------
  // Row 1, column 1.
  {
    label lj1(get_cost(li) + 1,
              {get_resources(li).min() + 1,
               get_resources(li).max()});
    lt(li, lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min(),
               get_resources(li).max() - 1});
    lt(li, lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() - 1});
    lt(li, lj3);
  }

  // Row 1, column 2.
  {
    label lj(get_cost(li) + 1, get_resources(li));
    lt(li, lj);
  }

  // Row 1, column 3.
  {
    label lj1(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max()});
    lt(li, lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min(),
               get_resources(li).max() + 1});
    lt(li, lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() + 1});
    lt(li, lj3);
  }

  // Row 1, column 4.
  {
    label lj1(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() - 1});
    lt(li, lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() + 1});
    lt(li, lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).min()});
    lt(li, lj3);

    label lj4(get_cost(li) + 1,
              {get_resources(li).max(),
               get_resources(li).max() + 1});
    lt(li, lj4);

    label lj5(get_cost(li) + 1,
              {get_resources(li).min() - 2,
               get_resources(li).min() - 1});
    lt(li, lj5);

    label lj6(get_cost(li) + 1,
              {get_resources(li).max() + 1,
               get_resources(li).max() + 2});
    lt(li, lj6);
  }

  // -----------------------------------------------------------------
  // Row 2, column 1.
  {
    label lj1(get_cost(li),
              {get_resources(li).min() + 1,
               get_resources(li).max()});
    lt(li, lj1);

    label lj2(get_cost(li),
              {get_resources(li).min(),
               get_resources(li).max() - 1});
    lt(li, lj2);

    label lj3(get_cost(li),
              {get_resources(li).min() + 1,
               get_resources(li).max() - 1});
    lt(li, lj3);
  }

  // Row 2, column 2.
  {
    label lj(get_cost(li), get_resources(li));
    eq(li, lj);
  }

  // Row 2, column 3.
  {
    label lj1(get_cost(li),
              {get_resources(li).min() - 1,
               get_resources(li).max()});
    gt(li, lj1);

    label lj2(get_cost(li),
              {get_resources(li).min(),
               get_resources(li).max() + 1});
    gt(li, lj2);

    label lj3(get_cost(li),
              {get_resources(li).min() - 1,
               get_resources(li).max() + 1});
    gt(li, lj3);
  }

  // Row 2, column 4.
  {
    // li < lj
    for(const auto &cu: worse_RI(get_resources(li)))
      {
        label lj(get_cost(li), cu);
        lt(li, lj);
      }

    // li > lj
    for(const auto &cu: better_RI(get_resources(li)))
      {
        label lj(get_cost(li), cu);
        gt(li, lj);
      }
  }

  // -----------------------------------------------------------------
  // Row 3, column 1.
  {
    label lj1(get_cost(li) - 1,
              {get_resources(li).min() + 1,
               get_resources(li).max()});
    gt(li, lj1);

    label lj2(get_cost(li) - 1,
              {get_resources(li).min(),
               get_resources(li).max() - 1});
    gt(li, lj2);

    label lj3(get_cost(li) - 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() - 1});
    gt(li, lj3);
  }

  // Row 3, column 2.
  {
    label lj(get_cost(li) - 1, get_resources(li));
    gt(li, lj);
  }

  // Row 3, column 3.
  {
    label lj1(get_cost(li) - 1,
              {get_resources(li).min() - 1,
               get_resources(li).max()});
    gt(li, lj1);

    label lj2(get_cost(li) - 1,
              {get_resources(li).min(),
               get_resources(li).max() + 1});
    gt(li, lj2);

    label lj3(get_cost(li) - 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() + 1});
    gt(li, lj3);
  }

  // Row 3, column 4.
  {
    label lj1(get_cost(li) - 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() - 1});
    gt(li, lj1);

    label lj2(get_cost(li) - 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() + 1});
    gt(li, lj2);

    label lj3(get_cost(li) - 1,
              {get_resources(li).min() - 1,
               get_resources(li).min()});
    gt(li, lj3);

    label lj4(get_cost(li) - 1,
              {get_resources(li).max(),
               get_resources(li).max() + 1});
    gt(li, lj4);

    label lj5(get_cost(li) - 1,
              {get_resources(li).min() - 2,
               get_resources(li).min() - 1});
    gt(li, lj5);

    label lj6(get_cost(li) - 1,
              {get_resources(li).max() + 1,
               get_resources(li).max() + 2});
    gt(li, lj6);
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
  // Row 1, column 1.
  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() + 1,
               get_resources(li).max()));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min(),
               get_resources(li).max() - 1));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() + 1,
               get_resources(li).max() - 1));

  // Row 1, column 2.
  s.emplace(get_cost(li) + 1, get_resources(li));

  // Row 1, column 3.
  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() - 1,
               get_resources(li).max()));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min(),
               get_resources(li).max() + 1));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() - 1,
               get_resources(li).max() + 1));

  // Row 1, column 4.
  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() - 1,
               get_resources(li).max() - 1));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() + 1,
               get_resources(li).max() + 1));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() - 3,
               get_resources(li).min()));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).max(),
               get_resources(li).max() + 4));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).min() - 4,
               get_resources(li).min() - 1));

  s.emplace(get_cost(li) + 1,
            CU(get_resources(li).max() + 1,
               get_resources(li).max() + 4));

  // -----------------------------------------------------------------
  // Row 2, column 1.
  s.emplace(get_cost(li),
            CU(get_resources(li).min() + 1,
               get_resources(li).max()));

  s.emplace(get_cost(li),
            CU(get_resources(li).min(),
               get_resources(li).max() - 1));

  s.emplace(get_cost(li),
            CU(get_resources(li).min() + 1,
               get_resources(li).max() - 1));

  // Row 2, column 4.
  for(const auto &cu: worse_RI(get_resources(li)))
    s.emplace(get_cost(li), cu);
  
  return s;
}

// *****************************************************************
// Test transitivity exchaustively.
// *****************************************************************

void
test_transitivity()
{
  // This label could be any, but with at least three units.
  label li(10, {10, 20});

  for(const auto &lj: worse(li))
    for(const auto &lk: worse(lj))
      {
        assert(li < lj && lj < lk);
        assert(li < lk);

        // Test the default implementation of >.
        assert(lk > lj && lj > li);
        assert(lk > li);

        // Test the default implementation of <=.
        assert(li <= lj && lj <= lk);
        assert(li <= lk);

        // Test the default implementation of >=.
        assert(lk >= lj && lj >= li);
        assert(lk >= li);
      }
}

// Test the better-or-equal incomparability to pinpoint the reason why
// this relation is intransitive.
//
// The relation is intransitive since:
//
// * labels are of different cost, but it is intransitive for labels of
// equal cost and incomparable RIs.
void
test_boe_incomparability()
{
  label li(10, {10, 20});

  // Check this: li < lj < lk
  //
  // The first relation is: li < lj
  // The second relation is: lj < lk

  // First relation: row 1, column 3
  for (const auto &rj: better_RI(get_resources(li)))
    {
      // The cost is be higher, the CU better.
      label lj(get_cost(li) + 1, rj);

      // Second relation: row 1, column 4
      for (const auto &rk: incomparable_RI(get_resources(lj)))
        {
          // The cost is higher, the CU incomparable.
          label lj(get_cost(lj) + 1, rk);
        }
    }
}

int
main()
{
  test_relations();
  test_transitivity();
  test_boe_incomparability();
}
