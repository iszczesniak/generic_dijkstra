#include "generic_label.hpp"
#include "units.hpp"

using label = generic_label<double, cunits<int>>;

using namespace std;

// *****************************************************************
// Test all relations
// *****************************************************************

void
test_relations()
{
  // This label could be any.
  label li(10, {10, 20});

  // Row 1, column 1.
  {
    label lj1(get_cost(li) + 1,
              {get_resource(li).min() + 1,
               get_resource(li).max()});
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resource(li).min(),
               get_resource(li).max() - 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resource(li).min() + 1,
               get_resource(li).max() - 1});
    assert(li < lj3);
  }

  // Row 1, column 2.
  {
    label lj(get_cost(li) + 1, get_resource(li));
    assert(li < lj);
  }

  // Row 1, column 3.
  {
    label lj1(get_cost(li) + 1,
              {get_resource(li).min() - 1,
               get_resource(li).max()});
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resource(li).min(),
               get_resource(li).max() + 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resource(li).min() - 1,
               get_resource(li).max() + 1});
    assert(li < lj3);
  }

  // Row 1, column 4.
  {
    label lj1(get_cost(li) + 1,
              {get_resource(li).min() - 1,
               get_resource(li).max() - 1});
    assert(li < lj1);

    label lj2(get_cost(li) + 1,
              {get_resource(li).min() + 1,
               get_resource(li).max() + 1});
    assert(li < lj2);

    label lj3(get_cost(li) + 1,
              {get_resource(li).min() - 1,
               get_resource(li).min()});
    assert(li < lj3);

    label lj4(get_cost(li) + 1,
              {get_resource(li).max(),
               get_resource(li).max() + 1});
    assert(li < lj4);

    label lj5(get_cost(li) + 1,
              {get_resource(li).min() - 2,
               get_resource(li).min() - 1});
    assert(li < lj5);

    label lj6(get_cost(li) + 1,
              {get_resource(li).max() + 1,
               get_resource(li).max() + 2});
    assert(li < lj6);
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
