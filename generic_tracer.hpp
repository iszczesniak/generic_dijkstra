#ifndef GENERIC_TRACER_HPP
#define GENERIC_TRACER_HPP

#include <list>

template <typename Permanent>
struct generic_tracer
{
  const Permanent &m_P;

  // That's the label type we're using.
  using label_type = typename Permanent::label_type;
  // The type of vertex data.
  using vd_type = typename Permanent::vd_type;
  // The path type.  The trace function needs it.
  using path_type = std::list<label_type>;

  generic_tracer(const Permanent &P): m_P(P)
  {
  }

  /**
   * Initialize the tracing.
   */
  auto
  init(const vd_type &ls) const
  {
    // Make sure there is at least one label.
    assert(!std::empty(ls));
    // Return the iterator to the first label.
    return ls.begin();
  }

  /**
   * Push the label to the path.
   */
  void
  push(path_type &p, typename vd_type::const_iterator li) const
  {
    // This is the label we process.
    const auto &l = *li;
    // Add the label to the path.
    p.push_front(l);
  }

  /**
   * We are given the iterator to the label that we process here.  We
   * require this is not the starting label.  The objective is to find
   * the previous label on the path.
   */
  auto
  advance(typename vd_type::const_iterator li) const
  {
    // This is the label we process.
    const auto &l = *li;
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
    const auto &ls = m_P[get_index(s)];
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
        // This is the next label iterator.
        return j;

    // We should never get here!
    assert(false);
  }

  const auto &
  operator[](typename vd_type::size_type index) const
  {
    return m_P[index];
  }
};

#endif // GENERIC_TRACER_HPP
