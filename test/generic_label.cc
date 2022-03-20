#include "generic_label.hpp"
#include "units.hpp"

using label = generic_label<double, CU>;

using namespace std;

// *****************************************************************
// Test all relations
// *****************************************************************

list<CU>
worse_CU(const CU &ri)
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

list<CU>
better_CU(const CU &ri)
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
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min(),
               get_resources(li).max() - 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() - 1});
    assert(li < lj3);
  }

  // Row 1, column 2.
  {
    label lj(get_cost(li) + 1, get_resources(li));
    assert(li < lj);
  }

  // Row 1, column 3.
  {
    label lj1(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max()});
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min(),
               get_resources(li).max() + 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() + 1});
    assert(li < lj3);
  }

  // Row 1, column 4.
  {
    label lj1(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).max() - 1});
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resources(li).min() + 1,
               get_resources(li).max() + 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resources(li).min() - 1,
               get_resources(li).min()});
    assert(li < lj3);

    label lj4(get_cost(li) + 1,
              {get_resources(li).max(),
               get_resources(li).max() + 1});
    assert(li < lj4);

    label lj5(get_cost(li) + 1,
              {get_resources(li).min() - 2,
               get_resources(li).min() - 1});
    assert(li < lj5);

    label lj6(get_cost(li) + 1,
              {get_resources(li).max() + 1,
               get_resources(li).max() + 2});
    assert(li < lj6);
  }

  // -----------------------------------------------------------------
  // Row 2, column 1.
  {
    label lj1(get_cost(li),
              {get_resources(li).min() + 1,
               get_resources(li).max()});
    assert(li < lj1);

    label lj2(get_cost(li),
              {get_resources(li).min(),
               get_resources(li).max() - 1});
    assert(li < lj2);

    label lj3(get_cost(li),
              {get_resources(li).min() + 1,
               get_resources(li).max() - 1});
    assert(li < lj3);
  }

  // Row 2, column 2.
  {
    label lj(get_cost(li), get_resources(li));
    assert(li == lj);
  }

  // Row 2, column 3.
  {
    label lj1(get_cost(li),
              {get_resources(li).min() - 1,
               get_resources(li).max()});
    assert(li > lj1);

    label lj2(get_cost(li),
              {get_resources(li).min(),
               get_resources(li).max() + 1});
    assert(li > lj2);

    label lj3(get_cost(li),
              {get_resources(li).min() - 1,
               get_resources(li).max() + 1});
    assert(li > lj3);
  }

  // Row 2, column 4.
  {
    // li < lj
    for(const auto &cu: worse_CU(get_resources(li)))
      {
        label lj(get_cost(li), cu);
        assert(li < lj);
      }

    // li > lj
    for(const auto &cu: better_CU(get_resources(li)))
      {
        label lj(get_cost(li), cu);
        assert(li > lj);
      }
  }
}

// *****************************************************************
// transitivity
// *****************************************************************

list<label>
worse(const label &li)
{
  list<label> l;

  return l;
}

void
test_transitivity()
{
  // This label could be any.
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


int
main()
{
  test_relations();
  test_transitivity();
}
