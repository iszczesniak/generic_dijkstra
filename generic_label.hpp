#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include <tuple>

// The label is a tuple (c, u, e) of cost c, units u, and edge e.  The
// label stores cost c and units u of using edge e.
//
// In the tuple we store an edge descriptor, not a vertex descriptor,
// so that we can allow for multigraphs (i.e. graphs with parallel
// edges).

template <typename Cost, typename Units, typename Edge>
struct generic_label
{
  Cost m_c;
  Units m_u;
  Descriptor<const Edge> m_ed;

  generic_label(const Cost &c, const Units &u, const Edge &e):
    m_c(c), m_u(u), m_ed(desc(e))
  {
  }

  generic_label(const Cost &c, Units &&u, const Edge &e):
    m_c(c), m_u(std::move(u)), m_ed(desc(e))
  {
  }

  generic_label(const generic_label &) = default;

  generic_label(generic_label &&) = default;

  generic_label &
  operator = (generic_label &&) = default;

  bool
  operator == (const generic_label &j) const
  {
    // We compare first the cost and target, since they are the most
    // likely to differ.  We compare the units at the very end,
    // because that comparison is time-consuming.
    return std::tie(m_c, m_ed, m_u) ==
      std::tie(j.m_c, j.m_ed, j.m_u);
  }

  bool
  operator != (const generic_label &j) const
  {
    return std::tie(m_c, m_ed, m_u) !=
      std::tie(j.m_c, j.m_ed, j.m_u);
  }

  // This operator is used by containers to establish the order
  // between labels.  All we really care about is the cost, and the
  // units.  The order of units matters, because it influences the
  // spectrum allocation policy.  Here the units are sorted by their
  // increasing min() numbers (the < operator of cunits), and so the
  // specturm allocation policy is the first-fit.
  //
  // To distinguish different labels, we need to take into account the
  // other label properties, and to this end we use the operator < of
  // a tuple.  Tuple should be optimized out, so there is no overhead
  // in using the tuple here.
  bool
  operator < (const generic_label &j) const
  {
    return std::tie(m_c, m_u, m_ed) <
      std::tie(j.m_c, j.m_u, j.m_ed);
  }

  // This "better than or equal to" operator is used by the
  // implementation of the generic Dijkstra algorithm
  bool
  operator <= (const generic_label &j) const
  {
    return m_c <= j.m_c && m_u.includes(j.m_u);
  }
};

template <typename Cost, typename Units, typename Edge>
auto
cost(const generic_label<Cost, Units, Edge> &l)
{
  return l.m_c;
}

template <typename Cost, typename Units, typename Edge>
auto
units(const generic_label<Cost, Units, Edge> &l)
{
  return l.m_u;
}

template <typename Cost, typename Units, typename Edge>
auto
get_edge(const generic_label<Cost, Units, Edge> &l)
{
  return *l.m_ed;
}

template <typename Cost, typename Units, typename Edge>
auto
target(const generic_label<Cost, Units, Edge> &l)
{
  return target(*l.m_ed);
}

#endif // GENERIC_LABEL_HPP
