#include "generic_label.hpp"
#include "units.hpp"

using label = generic_label<double, cunits<int>>;

using namespace std;

void
test_generic_label()
{
}

list<label>
worse(const label &li)
{
  list<label> l;

  return l;
}

// *****************************************************************
// transitivity
// *****************************************************************
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
}

int
main()
{
  test_generic_label();
  test_transitivity();
  test_relations();
}
