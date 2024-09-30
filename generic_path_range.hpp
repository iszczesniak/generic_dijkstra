#ifndef GENERIC_PATH_RANGE_HPP
#define GENERIC_PATH_RANGE_HPP

#include "graph.hpp"

#include <iostream>

// Iterates over the labels of a path.
template <typename Permanent>
struct generic_path_iterator
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The label we currently point to.  The label is stored in m_P.
  const label_type *m_lp;

  generic_path_iterator(const Permanent &P, const label_type &l):
    m_P(P), m_lp(&l)
  {
  }

  const label_type &
  operator * ()
  {
    return *m_lp;
  }

  /**
   * Find the previous label of the path.
   */
  auto &
  operator ++ ()
  {
    // This is the label we process.
    const auto &l = *m_lp;
    // This is the edge of the label.
    const auto &e = get_edge(l);
    // The weight of the label.
    const auto &w = get_weight(l);
    // These are the resources of the label.
    const auto &r = get_resources(l);
    // The edge weight.
    const auto &ew = get_weight(e);
    // The edge source.
    const auto &s = get_source(e);

    // Find the labels at node s.
    const auto &ls = m_P[get_key(s)];
    assert(!ls.empty());

    // We look for a preceeding label at node s.  The loop condition
    // is true, because the loop should always be terminated by the
    // return statement.  Nonetheless, we make sure with the assertion
    // that we never reach the end.
    for(auto j = ls.begin(); assert(j != ls.end()), true; ++j)
      // Note that we are not subtracting (which is more natural,
      // since we are tracing back the path, and decreasing the
      // weight), but adding the weights, because during the search we
      // use the addition only.  Since we might be dealing with
      // floating point numbers (i.e., weight may be float or double),
      // this could fail:
      //
      //  ew == w - weight(*j)
      //
      // And this will be fine:
      //
      //  get_weight(*j) + ew == w
      if (get_weight(*j) + ew == w && includes(get_resources(*j), r))
        {
          // This is the next label iterator.
          m_lp = &*j;

          return *this;
        }

    // We should never get here!
    assert(false);
  }
};

template <typename Permanent>
bool
operator == (const generic_path_iterator<Permanent> &i1,
             const generic_path_iterator<Permanent> &i2)
{
  return *(i1.m_lp) == *(i2.m_lp);
}

template <typename Permanent>
struct generic_path_range
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The label we start with.
  const label_type &m_l;
  // The initial label of the search, at which we end the itaration.
  const label_type &m_initial;

  generic_path_range(const Permanent &P,
                      const label_type &l, const label_type &initial):
    m_P(P), m_l(l), m_initial(initial)
  {
  }

  auto
  begin() const
  {
    return generic_path_iterator(m_P, m_l);
  }

  auto
  end() const
  {
    return generic_path_iterator(m_P, m_initial);
  }
};

#endif // GENERIC_PATH_RANGE_HPP
