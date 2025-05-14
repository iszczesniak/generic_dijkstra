#ifndef GENERIC_PATH_RANGE_HPP
#define GENERIC_PATH_RANGE_HPP

#include "graph_interface.hpp"

#include <functional>

// Iterates over the labels of a path.
template <typename Permanent, typename Functor>
struct generic_path_iterator
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The functor that was used to create labels in m_P.
  const Functor &m_f;
  // The label we currently point to.  The label is stored in m_P.
  std::reference_wrapper<const label_type> m_l;

  generic_path_iterator(const Permanent &P, const Functor &f,
                        const label_type &l):
    m_P(P), m_f(f), m_l(l)
  {
  }

  const label_type &
  operator * ()
  {
    return m_l;
  }

  // Find the label that precedes label m_l, assuming there is one.
  //
  //          Find:                   Given:
  //          label pl                label m_l
  // () ----- edge pe ----> (s) ----- edge e ----> ()

  auto &
  operator ++ ()
  {
    // The edge of label m_l.
    const auto &e = get_edge(m_l.get());
    // The source of edge e.
    const auto &s = get_source(e);

    // These are the labels for the source of label m_l.
    const auto &labels = m_P[get_key(s)];
    assert(!labels.empty());

    // Here we keep track of what we found.
    const label_type *ptr = nullptr;

    // Now we iterate over the labels to see which label pl was used
    // to produce label m_l.
    for (const auto &pl: labels)
      {
        // The edge of label pl.
        const auto &pe = get_edge(pl);

        // The target of pe is the same as the source of e.
        assert(get_target(pe) == get_source(e));

        // Candidate labels.
        auto cls = m_f(pl, e);

        for(const auto &cl: cls)
          if (cl == m_l)
            {
              // Exactly one of the candidate labels must equal to
              // label m_l, so ptr must be nullptr.
              assert(ptr == nullptr);
              ptr = &pl;
            }
      }

    m_l = *ptr;

    return *this;
  }

};

template <typename Permanent, typename Functor>
bool
operator == (const generic_path_iterator<Permanent, Functor> &i1,
             const generic_path_iterator<Permanent, Functor> &i2)
{
  // With C++26, I shall say: get_edge(i1.m_l) == get_edge(i2.m_l);
  return get_edge(i1.m_l.get()) == get_edge(i2.m_l.get());
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
  const label_type &m_last;
  // The initial label of the search, at which we end the itaration.
  const label_type &m_init;

  generic_path_range(const Permanent &P, const Functor &f,
                     const label_type &last, const label_type &init):
    m_P(P), m_f(f), m_last(last), m_init(init)
  {
  }

  auto
  begin() const
  {
    return generic_path_iterator(m_P, m_f, m_last);
  }

  auto
  end() const
  {
    return generic_path_iterator(m_P, m_f, m_init);
  }
};

#endif // GENERIC_PATH_RANGE_HPP
