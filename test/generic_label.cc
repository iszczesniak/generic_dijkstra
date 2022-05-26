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

  for(auto i = ri.min(); i < ri.max(); ++i)
    for(auto j = i + 1; j <= ri.max(); ++j)
      if (CU(i, j) != ri)
        l.insert(CU(i, j));

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

void
test_icl_RIs()
{
  CU omega(0, 4);

  auto l1 = icl_RIs(CU(0, 2), omega);
  assert(l1.empty());

  auto l2 = icl_RIs(CU(1, 2), omega);
  assert(l2.size() == 1);
  assert(l2.contains(CU(0, 1)));

  auto l3 = icl_RIs(CU(1, 3), omega);
  assert(l3.size() == 2);
  assert(l3.contains(CU(0, 1)));
  assert(l3.contains(CU(0, 2)));

  auto l4 = icl_RIs(CU(1, 4), omega);
  assert(l4.size() == 3);
  assert(l4.contains(CU(0, 1)));
  assert(l4.contains(CU(0, 2)));
  assert(l4.contains(CU(0, 3)));
}

// Returns RIs rj such that ri || rj, rj is on the right of ri.
auto
icr_RIs(const CU &ri, const CU &omega)
{
  assert(includes(omega, ri));

  set<CU> l;

  for(auto j = omega.max(); ri.max() < j; --j)
    for(auto i = j - 1; ri.min() < i; --i)
      l.insert(CU(i, j));

  return l;
}

void
test_icr_RIs()
{
  CU omega(0, 4);

  auto l1 = icr_RIs(CU(2, 4), omega);
  assert(l1.empty());

  auto l2 = icr_RIs(CU(2, 3), omega);
  assert(l2.size() == 1);
  assert(l2.contains(CU(3, 4)));

  auto l3 = icr_RIs(CU(1, 3), omega);
  assert(l3.size() == 2);
  assert(l3.contains(CU(3, 4)));
  assert(l3.contains(CU(2, 4)));

  auto l4 = icr_RIs(CU(0, 3), omega);
  assert(l4.size() == 3);
  assert(l4.contains(CU(3, 4)));
  assert(l4.contains(CU(2, 4)));
  assert(l4.contains(CU(1, 4)));
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

// *****************************************************************
// Test all relations.
// *****************************************************************

void
test_relations()
{
  CU omega(0, 30);
  // This label could be any.
  label li(10, {10, 20});

  // Column 1.
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

auto
worse_labels(const label &li, const CU &omega)
{
  set<label> s;

  // Column 1. Row 1.
  for(auto &rj: sup_RIs(get_resources(li), omega))
    s.emplace(get_cost(li) + 1, rj);

  // Column 2. Row 1.
  s.emplace(get_cost(li) + 1, get_resources(li));

  // Column 3.
  for(auto &rj: sub_RIs(get_resources(li)))
    {
      // Row 1.
      s.emplace(get_cost(li) + 1, rj);
      // Row 2.
      s.emplace(get_cost(li), rj);
    }

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

// For label li, the function produces incomparable labels lj.  There
// are five cases (from a to e) with these conditions:
//
// a. column 1, row 1: cost(li) < cost(lj) and RI(li) \subset RI(lj)
//
// b. column 3, row 3: cost(li) > cost(lj) and RI(li) \supset RI(lj)
//
// c. column 4, row 1: cost(li) < cost(lj) and RI(li) || RI(lj)
//
// d. column 4, row 2: cost(li) = cost(lj) and RI(li) || RI(lj)
//
// e. column 4, row 3: cost(li) > cost(lj) and RI(li) || RI(lj)

auto
incomparable_labels(const string &cases, const label &li,
                    const CU &omega)
{
  set<label> l;

  // a.  Column 1.  Row 1.
  if (cases.contains('a'))
    for(auto &rj: sup_RIs(get_resources(li), omega))
      l.emplace(get_cost(li) + 1, rj);

  // b.  Column 3.  Row 3.
  if (cases.contains('b'))
    for(auto &rj: sub_RIs(get_resources(li)))
      l.emplace(get_cost(li) - 1, rj);

  // c.  Column 4.  Row 1.
  if (cases.contains('c'))
    for(auto &rj: incomparable_RIs(get_resources(li), omega))
      l.emplace(get_cost(li) + 1, rj);

  // d.  Column 4.  Row 2.
  if (cases.contains('d'))
    for(auto &rj: incomparable_RIs(get_resources(li), omega))
      l.emplace(get_cost(li), rj);

  // e.  Column 4.  Row 3.
  if (cases.contains('e'))
    for(auto &rj: incomparable_RIs(get_resources(li), omega))
      l.emplace(get_cost(li) - 1, rj);

  return l;
}

// This function tests the relations between li and lk, provided li ||
// lj and lj || lk.  Returns true if the relations are as given in
// "expected", where the recognized characters are:
//
// * '<' for better than,
// * '=' for equal to,
// * '>' for worse than,
// * '|' for incomparable with.
//
// Having label li, labels lj are produced for the cases (from a to e)
// in s1.  Having label lj, labels lk are produced for cases (from a
// to e) in s2.
bool
test_case(const string &s1, const string &s2, const string &expected)
{
  CU omega(0, 10);

  // Better than.
  bool bt = false;
  // Equal.
  bool eq = false;
  // Worse than.
  bool wt = false;
  // Incomparable.
  bool ic = false;

  for(const auto &ri: sub_RIs(omega))
    {
      label li(10, ri);

      for(const auto &lj: incomparable_labels(s1, li, omega))
        {
          assert(is_incomparable(li, lj));

          for(const auto &lk: incomparable_labels(s2, lj, omega))
            {
              assert(is_incomparable(lj, lk));

              bt |= (boe(li, lk) && li != lk);
              eq |= (li == lk);
              wt |= (boe(lk, li) && li != lk);
              ic |= is_incomparable(li, lk);

              // The incomparability is symmetric: both hold or not.
              assert (!(is_incomparable(li, lk) ^ is_incomparable(lk, li)));
            }
        }
    }

  if (!expected.contains('<'))
    bt = !bt;
  if (!expected.contains('='))
    eq = !eq;
  if (!expected.contains('>'))
    wt = !wt;
  if (!expected.contains('|'))
    ic = !ic;
  
  return bt && eq && wt && ic;
}

// The better-or-equal incomparability is intransitive, i.e., even
// though both li || lj and lj || lk are true, li || lk does not
// always hold, because the conditions for incomparability (cases from
// a to e above) are not met.

void
test_intran_boe_incomp()
{
  // *****************************************************************
  // Here the incomparability relation is transitive.

  assert(test_case("a", "acd", "|"));
  assert(test_case("b", "bde", "|"));
  assert(test_case("c", "a", "|"));
  assert(test_case("d", "ab", "|"));
  assert(test_case("e", "b", "|"));

  // *****************************************************************
  // The incomparability relation is transitive in the other cases.

  // In the following cases we can have any relation (\prec, ==,
  // \succ, ||), because independently:
  //
  // * costs can be in any relation:
  //   - cost(li) < cost(lk)
  //   - cost(li) == cost(lk)
  //   - cost(li) > cost(lk)
  //
  // * RIs can be in any relation:
  //   - RI(li) \supset RI(lk)
  //   - RI(li) == RI(lk)
  //   - RI(li) \subset RI(lk)

  assert(test_case("a", "b", "<=>|"));
  assert(test_case("b", "a", "<=>|"));

  // In the following cases we can have \succ and ||, but not == nor
  // \prec, because:

  assert(test_case("a", "e", ">|"));
  assert(test_case("e", "a", ">|"));

  // In the following cases we can have \prec and ||, but not == nor
  // \succ, because:

  assert(test_case("b", "c", "<|"));
  assert(test_case("c", "b", "<|"));

  // *****************************************************************
  // In the following we have the cases of c, d, and e only.  In these
  // cases RIs are incomparable, and so the relations between RIs can
  // be any, i.e.:
  //
  // * RI(li) \supset RI(lk)
  // * RI(li) == RI(lk)
  // * RI(li) \subset RI(lk)
  // * RI(li) || RI(lk)
  //
  // Therefore the expected relations depend mainly on the cost.

  // In the following cases we can have only relations \prec and ||,
  // because the cost relation can only be:
  //
  // * cost(li) < cost(lk)

  assert(test_case("cd", "c", "<|"));
  assert(test_case("c", "cd", "<|"));

  // In the following cases we can have any relation (\prec, ==,
  // \succ, ||), because the cost relation can only be:
  //
  // * cost(li) < cost(lk)
  // * cost(li) == cost(lk)
  // * cost(li) > cost(lk)

  assert(test_case("c", "e", "<=>|"));
  assert(test_case("e", "c", "<=>|"));

  // In the following case the cost relation can only be:
  //
  // * cost(li) == cost(lk)
  //
  // However, the relations between RIs can be any, and therefore the
  // relation between the labels can be any.
  
  assert(test_case("d", "d", "<=>|"));

  // In the following cases we can have only relations \succ and ||,
  // because the cost relation can only be:
  //
  // * cost(li) > cost(lk)

  assert(test_case("de", "e", "|>"));
  assert(test_case("e", "de", "|>"));
}

int
main()
{
  test_sub_RIs();
  test_sup_RIs();
  test_icl_RIs();
  test_icr_RIs();

  test_relations();
  test_transitivity();
  test_intran_boe_incomp();
}
