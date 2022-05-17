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

// a.  Column 1.  Row 3.
auto
incomparable_labels_a(const label &li)
{
  list<label> l;

  for(auto &rj: sub_RIs(get_resources(li)))
    l.push_back(label{get_cost(li) - 1, rj});

  return l;
}

// b.  Column 3.  Row 1.
auto
incomparable_labels_b(const label &li, const CU &omega)
{
  list<label> l;

  for(auto &rj: sup_RIs(get_resources(li), omega))
    l.push_back(label{get_cost(li) + 1, rj});

  return l;
}

// C.  Column 4.  Row 1.
auto
incomparable_labels_c(const label &li, const CU &omega)
{
  list<label> l;

  for(auto &rj: incomparable_RIs(get_resources(li), omega))
    l.push_back(label{get_cost(li) + 1, rj});

  return l;
}

// D.  Column 4.  Row 2.
auto
incomparable_labels_d(const label &li, const CU &omega)
{
  list<label> l;

  for(auto &rj: incomparable_RIs(get_resources(li), omega))
    l.push_back(label{get_cost(li), rj});

  return l;
}

// E.  Column 4.  Row 3.
auto
incomparable_labels_e(const label &li, const CU &omega)
{
  list<label> l;

  for(auto &rj: incomparable_RIs(get_resources(li), omega))
    l.push_back(label{get_cost(li) - 1, rj});

  return l;
}

// Test the better-or-equal incomparability to pinpoint the reason why
// this relation is intransitive.
//
// The relation is intransitive since:
//
// * labels are of different cost, but it is intransitive for labels
// of equal cost and incomparable RIs.

void
test_intran_boe_incomp()
{
  CU omega(0, 10);

  // *****************************************************************
  //
  // The incomparability relation is transitive for:
  //
  // * a: a, d, e,
  //
  // * b: b, c, d
  //
  // * c: b
  //
  // * d: a, b
  //
  // * e: a
  for(const auto &ri: sub_RIs(omega))
    {
      label li(10, ri);

      // Row a.
      for(const auto &lj: incomparable_labels_a(li))
        {
          assert(is_incomparable(li, lj));

          // Column a.
          for(const auto &lk: incomparable_labels_a(lj))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }

          // Column d.
          for(const auto &lk: incomparable_labels_d(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }

          // Column e.
          for(const auto &lk: incomparable_labels_e(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }
        }

      // Row b.
      for(const auto &lj: incomparable_labels_b(li, omega))
        {
          assert(is_incomparable(li, lj));

          // Column b.
          for(const auto &lk: incomparable_labels_b(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }

          // Column c.
          for(const auto &lk: incomparable_labels_c(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }

          // Column d.
          for(const auto &lk: incomparable_labels_d(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }
        }

      // Row c.
      for(const auto &lj: incomparable_labels_c(li, omega))
        {
          assert(is_incomparable(li, lj));

          // Column b.
          for(const auto &lk: incomparable_labels_b(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }
        }

      // Row d.
      for(const auto &lj: incomparable_labels_d(li, omega))
        {
          assert(is_incomparable(li, lj));

          // Column a.
          for(const auto &lk: incomparable_labels_a(lj))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }

          // Column b.
          for(const auto &lk: incomparable_labels_b(lj, omega))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }
        }

      // Row e.
      for(const auto &lj: incomparable_labels_e(li, omega))
        {
          assert(is_incomparable(li, lj));

          // Column a.
          for(const auto &lk: incomparable_labels_a(lj))
            {
              assert(is_incomparable(lj, lk));
              assert(is_incomparable(li, lk));
            }
        }
    }

  // *****************************************************************
  //
  // The incomparability relation is transitive in the other cases.

  // Cases where all relations can take place:
  //
  // * a: b
  //
  // * b: a
  //
  // We can have any relation (\prec, ==, \succ, \parallel), because
  // independently:
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

  // The a: b case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_a(li))
          {
            assert(is_incomparable(li, lj));

            // Column b.
            for(const auto &lk: incomparable_labels_b(lj, omega))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(bt);
    assert(eq);
    assert(wt);
    assert(ic);
  }

  // The b: a case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_b(li, omega))
          {
            assert(is_incomparable(li, lj));

            // Column b.
            for(const auto &lk: incomparable_labels_a(lj))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(bt);
    assert(eq);
    assert(wt);
    assert(ic);
  }

  // Cases for:
  //
  // * a: c
  //
  // * c: a
  //
  // We have relations: \prec and \parallel, but not == nor \succ.

  // The a: c case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_a(li))
          {
            assert(is_incomparable(li, lj));

            // Column c.
            for(const auto &lk: incomparable_labels_c(lj, omega))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(bt);
    assert(!eq);
    assert(!wt);
    assert(ic);
  }

  // The c: a case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_c(li, omega))
          {
            assert(is_incomparable(li, lj));

            // Column b.
            for(const auto &lk: incomparable_labels_a(lj))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(bt);
    assert(!eq);
    assert(!wt);
    assert(ic);
  }

  // Cases for:
  //
  // * b: e
  //
  // * e: b
  //
  // We have relations: \succ and \parallel, but not == nor \prec.

  // The b: e case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_b(li, omega))
          {
            assert(is_incomparable(li, lj));

            // Column c.
            for(const auto &lk: incomparable_labels_e(lj, omega))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(!bt);
    assert(!eq);
    assert(wt);
    assert(ic);
  }

  // The e: b case.
  {
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

        // Row a.
        for(const auto &lj: incomparable_labels_e(li, omega))
          {
            assert(is_incomparable(li, lj));

            // Column b.
            for(const auto &lk: incomparable_labels_b(lj, omega))
              {
                assert(is_incomparable(lj, lk));

                bt |= (boe(li, lk) && li != lk);
                eq |= (li == lk);
                wt |= (boe(lk, li) && li != lk);
                ic |= is_incomparable(li, lk);
              }
          }
      }

    assert(!bt);
    assert(!eq);
    assert(wt);
    assert(ic);
  }
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
