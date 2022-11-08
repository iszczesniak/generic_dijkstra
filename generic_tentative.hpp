#ifndef GENERIC_TENTATIVE_HPP
#define GENERIC_TENTATIVE_HPP

#include <cassert>
#include <set>
#include <vector>

// The container type for storing the generic tentative labels.
template <typename Label>
struct generic_tentative: std::vector<std::set<Label>>
{
  // The label type
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::set<label_type>;
  // The type of the vector of vertex data.
  using base = std::vector<vd_type>;
  // The size_type of the base.
  using size_type = typename base::size_type;
  // The weight type of the label.
  using weight_type = Weight<label_type>;
  // The index type of the vertex.
  using index_type = Index<Vertex<Edge<label_type>>>;

  // The priority queue element type.
  using pqet = std::pair<weight_type, index_type>;

  // The priority queue.
  std::set<pqet> m_pq;

  // Vetext to weight reverse look up of the priority queue elements.
  std::vector<std::optional<weight_type>> m_v2c;

  // The constructor builds a vector of data for each vertex.
  generic_tentative(size_type count): base(count), m_v2c(count)
  {
  }

  // This function pushes a new label, and returns a reference to it
  // in the container.
  template <typename T>
  const auto &
  push(T &&l)
  {
    // The index of the target vertex.
    auto ti = get_index(get_target(l));
    auto &vd = generic_tentative::operator[](ti);
    auto [i, s] = vd.insert(std::forward<T>(l));
    // Make sure the insertion was successful.
    assert(s);

    if (i == vd.begin())
      {
        auto c = get_weight(l);
        // There already can be an element in the queue for tk.
        auto &o = m_v2c[ti];
        if (o)
          // Erase the former element from the queue.
          m_pq.erase({*o, ti});
        m_pq.insert({c, ti});
        o = c;
      }

    return *i;
  }

  bool
  empty() const
  {
    return m_pq.empty();
  }

  label_type
  pop()
  {
    assert(!m_pq.empty());
    const auto [c, ti] = *m_pq.begin();
    m_pq.erase(m_pq.begin());
    auto &vd = this->operator[](ti);
    assert(!vd.empty());
    auto nh = vd.extract(vd.begin());
    assert(get_weight(nh.value()) == c);
    auto &o = m_v2c[ti];
    // If there is other label for ti, put it into the queue.
    if (!vd.empty())
      {
        const auto &nc = get_weight(*vd.begin());
        assert(get_index(get_target(*vd.begin())) == ti);
        m_pq.insert({nc, ti});
        o = nc;
      }
    else
      o.reset();

    return std::move(nh.value());
  }
};

/**
 * Is there in T a label that is better than or equal to label j?
 */
template <typename Label>
bool
has_better_or_equal(const generic_tentative<Label> &T, const Label &j)
{
  // We could go for the easy implementation where we iterate for each
  // label i, and compare it to label j.  But we take advantage of the
  // fact that the elements in the set are sorted by weight first.

  // Iterate over all tentative labels.
  for (const auto &i: T[get_index(get_target(j))])
    {
      // Stop searching when we reach a label with a higher weight.  If
      // the weight of label i is higher than the weight of label j, then
      // label i (and the labels in the vector that follow) cannot be
      // better or equal (they can be incomparable or worse).
      if (get_weight(i) > get_weight(j))
        break;

      // Is label i better than or equal to label j?
      if (i <= j)
        return true;
    }

  return false;
}

/**
 * Purge from queue Q those labels which are worse than label j.
 */
template <typename Label>
void
purge_worse(generic_tentative<Label> &T, const Label &j)
{
  auto &Tt = T[get_index(get_target(j))];

  // We could go for the easy implementation where we iterate for each
  // label i and compare it to j.  But we take advantage of the fact
  // that the elements in the set are sorted by weight first.  We
  // iterate in the reverse order!
  for(auto r = Tt.rbegin(); r != Tt.rend();)
    {
      const auto &i = *r;

      // Stop searching when we reach label i with the weight lower than
      // the weight of label j.  If the weight of label i is lower than
      // the weight of label j, then label i (and the labels in the set
      // that follow) cannot be worse (they can be better or
      // incomparable).
      if (get_weight(i) < get_weight(j))
        break;

      // Make sure labels i and j are not equal.  We can make this
      // assertion here, because we are not inserting equal labels
      // into the priority queue.  We need this assertion here, so
      // that we can safely use the <= operator below.
      assert(!(get_weight(i) == get_weight(j) &&
               get_units(i) == get_units(j)));

      // To check whether label i is worse then j, we use the <=
      // operator, because we made sure the labels are not equal.
      if (j <= i)
        // We want to remove label i, and we're going to use iterator
        // r.  We can safely remove the element pointed to by r,
        // because the base iterator points to the element next to r.
        // This erasure does not invalidate the base iterator of r.
        // Note that we do not increment r, because after the erasure,
        // r will already point to the next element.
        Tt.erase(--(r.base()));
      else
        ++r;
    }
}

#endif // GENERIC_TENTATIVE_HPP
