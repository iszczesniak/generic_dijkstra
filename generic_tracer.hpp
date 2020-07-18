#ifndef GENERIC_TRACER_HPP
#define GENERIC_TRACER_HPP

#include "graph.hpp"

template <typename Path, typename Permanent, typename Units>
struct generic_tracer
{
  // That's the label type we're using.
  using label_t = typename Permanent::label_t;
  // The type of vertex data.
  using vd_t = typename Permanent::vd_t;
  // The path type.  The trace function needs it.
  using path_t = Path;

  // The number of contigous units.
  int m_ncu;

  // The units of the path.
  Units m_units;

  generic_tracer(int ncu):
    m_ncu(ncu), m_units()
  {
  }

  /**
   * Initialize the tracing.
   */
  auto
  init(path_t &p, const vd_t &ls)
  {
    // Make sure there is at least one label.
    assert(!std::empty(ls));
    // We just take the first label.
    const auto &l = ls.front();
    // Get the units for the path.
    p.first = m_units = units(l);
    // Return the iterator to the first (and the only) label.
    return ls.begin();
  }

  /**
   * Push the label to the path.
   */
  void
  push(path_t &p, typename vd_t::const_iterator li)
  {
    // This is the label we process.
    const auto &l = *li;
    // The edge of the label.
    const auto &e = get_edge(l);
    // Add the label's edge to the path.
    p.second.push_front(e);
  }

  /**
   * We are given the iterator to the label that we process here.  We
   * require this is not the starting label.  The objective is to find
   * the previous label on the path.
   */
  auto
  advance(const Permanent &P, typename vd_t::const_iterator li)
  {
    // This is the label we process.
    const auto &l = *li;
    // The cost of the label.
    const auto &c = cost(l);
    // This is the edge of the label.
    const auto &e = get_edge(l);
    // The edge cost.
    auto ec = cost(e);
    // The edge source.
    const auto &s = source(e);

    // Find the labels at node s.
    const auto &ls = P[s];
    assert(!ls.empty());

    // We look for a preceeding label at node s.  The loop condition
    // is true, because the loop should always be terminated by the
    // return statement.  Nonetheless, we make sure with the assertion
    // that we never reach the end.
    for(auto j = ls.begin(); assert(j != ls.end()), true; ++j)
      // Note that we are not subtracting (which is more natural,
      // since we are tracing back the path, and decreasing the cost),
      // but adding the costs, because during the search we use the
      // addition only.  Since we might be dealing with floating point
      // numbers (i.e., cost may be float or double), this could fail:
      //
      //  ec == c - cost(*j)
      //
      // And this will be fine:
      //
      //  cost(*j) + ec == c
      if (cost(*j) + ec == c && units(*j).includes(m_units))
        // This is the next label iterator.
        return j;

    // We should never get here!
    assert(false);
  }
};

#endif // GENERIC_TRACER_HPP
