#ifndef GENERIC_PATH_RANGE_HPP
#define GENERIC_PATH_RANGE_HPP

#include "graph_interface.hpp"

// Iterates over the labels of a path.
template <typename Permanent, typename Functor>
struct generic_path_iterator
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The functor that was used to created labels in m_P.
  const Functor &m_f;
  // The label we currently point to.  The label is stored in m_P.
  const label_type *m_lp;

  generic_path_iterator(const Permanent &P, const Functor &f,
                        const label_type &l):
    m_P(P), m_f(f), m_lp(&l)
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
    const auto &il = *m_lp;
    // This is the edge of the label.
    const auto &ie = get_edge(il);
    // The source vertex of the edge.
    const auto &is = get_source(ie);

    // Find the labels at vertex is.
    const auto &jls = m_P[get_key(is)];
    assert(!jls.empty());

    // We look for a preceeding label.
    for(const auto &jl: jls)
      {
        // The candidate labels.
        const auto &ls = m_f(jl, ie);

        int counter = 0;

        for(const auto &l: ls)
          if (l == il)
            {
              // This is the next label iterator.
              m_lp = &jl;
              ++counter;
            }

        assert(counter == 1);

        return *this;
      }

    // We should never get here!
    assert(false);
  }
};

template <typename Permanent, typename Functor>
bool
operator == (const generic_path_iterator<Permanent, Functor> &i1,
             const generic_path_iterator<Permanent, Functor> &i2)
{
  return *(i1.m_lp) == *(i2.m_lp);
}

template <typename Permanent, typename Functor>
struct generic_path_range
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The functor that was used to created labels in m_P.
  const Functor &m_f;
  // The label we start with.
  const label_type &m_l;
  // The initial label of the search, at which we end the itaration.
  const label_type &m_initial;

  generic_path_range(const Permanent &P, const Functor &f,
                     const label_type &l, const label_type &initial):
    m_P(P), m_f(f), m_l(l), m_initial(initial)
  {
  }

  auto
  begin() const
  {
    return generic_path_iterator(m_P, m_f, m_l);
  }

  auto
  end() const
  {
    return generic_path_iterator(m_P, m_f, m_initial);
  }
};

#endif // GENERIC_PATH_RANGE_HPP
