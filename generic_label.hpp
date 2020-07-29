#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include <ostream>
#include <tuple>

// The label has cost c, units u, and edge e.  The label stores cost c
// and units u of using edge e.
//
// We store an edge, not a vertex, so that we can allow for
// multigraphs (i.e. graphs with parallel edges).

template <typename Cost, typename Units, typename Edge>
struct generic_label
{
  Cost m_c;
  Units m_u;
  Edge m_e;

  generic_label(Cost c, Units u, Edge e):
    m_c(c), m_u(u), m_e(e)
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
    return std::tie(m_c, m_e, m_u) ==
      std::tie(j.m_c, j.m_e, j.m_u);
  }

  bool
  operator != (const generic_label &j) const
  {
    return std::tie(m_c, m_e, m_u) !=
      std::tie(j.m_c, j.m_e, j.m_u);
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
    return std::tie(m_c, m_u, m_e) <
      std::tie(j.m_c, j.m_u, j.m_e);
  }

  // This "better than or equal to" operator is used by the
  // implementation of the generic Dijkstra algorithm
  bool
  operator <= (const generic_label &j) const
  {
    return m_c <= j.m_c &&
      std::includes(m_u.begin(), m_u.end(),
                    j.m_u.begin(), j.m_u.end());
  }
};

template <typename Cost, typename Units, typename Edge>
const auto &
cost(const generic_label<Cost, Units, Edge> &l)
{
  return l.m_c;
}

template <typename Cost, typename Units, typename Edge>
const auto &
units(const generic_label<Cost, Units, Edge> &l)
{
  return l.m_u;
}

template <typename Cost, typename Units, typename Edge>
const auto &
edge(const generic_label<Cost, Units, Edge> &l)
{
  return l.m_e;
}

template <typename Cost, typename Units, typename Edge>
const auto &
target(const generic_label<Cost, Units, Edge> &l)
{
  return target(l.m_e);
}

template <typename Cost, typename Units, typename Edge>
std::ostream &
operator<<(std::ostream &out,
           const generic_label<Cost, Units, Edge> &l)
{
  out << "label(cost = " << cost(l)
      << ", units = " << units(l)
      << ", edge = " << edge(l) << ")";

    return out;
}

// *******************************************************************
// The cost traits

template <typename Cost, typename Units, typename Edge>
struct cost_traits<generic_label<Cost, Units, Edge>>
{
  using type = Cost;
};

// *******************************************************************
// The edge traits

template <typename Cost, typename Units, typename Edge>
struct edge_traits<generic_label<Cost, Units, Edge>>
{
  using type = Edge;
};

#endif // GENERIC_LABEL_HPP
