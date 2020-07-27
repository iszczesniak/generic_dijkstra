#ifndef GENERIC_TENTATIVE_HPP
#define GENERIC_TENTATIVE_HPP

#include "generic_label.hpp"

#include <cassert>
#include <set>
#include <vector>

// The container type for storing the generic tentative labels.  A
// vertex can have many shared (with the priority queue) labels or
// none.  For each vertex we store a container or shared_ptrs to
// labels, because we create a weak_ptr in the priority queue.
template <typename Graph, typename Cost, typename Units>
struct generic_tentative:
  std::vector<std::set<generic_label<Cost, Units, Edge<Graph>>>>
{
  // That's the label type we're using.
  using label_type = generic_label<Cost, Units, Edge<Graph>>;
  // The type of data a vertex has.
  using vd_type = std::set<label_type>;
  // The type of the vector of vertex data.
  using base = std::vector<vd_type>;
  // The size_type of the base.
  using size_type = typename base::size_type;
  // The type of the vertex index.
  using index_type = Index<Vertex<Graph>>;

  // The priority queue element type.
  using pqet = std::pair<Cost, index_type>;

  // The priority queue.
  std::set<pqet> m_pq;

  // Vetext to cost reverse look up of the priority queue elements.
  std::vector<std::optional<Cost>> m_v2c;

  // The constructor builds a vector of data for each vertex.
  generic_tentative(size_type count): base(count), m_v2c(count)
  {
  }

  // This function pushes a new label.
  template <typename T>
  void
  push(T &&l)
  {
    // The index of the target vertex.
    auto ti = index(target(l));
    auto &vd = this->operator[](ti);
    auto [i, s] = vd.insert(std::forward<T>(l));
    // Make sure the insertion was successful.
    assert(s);
    if (i == vd.begin())
      {
        Cost c = get_cost(l);
        // There already can be an element in the queue for tk.
        auto &o = m_v2c[ti];
        if (o)
          // Erase the former element from the queue.
          m_pq.erase({*o, ti});
        m_pq.insert({c, ti});
        o = c;
      }
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
    assert(get_cost(nh.value()) == c);
    auto &o = m_v2c[ti];
    // If there is other label for ti, put it into the queue.
    if (!vd.empty())
      {
        const auto &nc = get_cost(*vd.begin());
        assert(index(target(*vd.begin())) == ti);
        m_pq.insert({nc, ti});
        o = nc;
      }
    else
      o.reset();

    return std::move(nh.value());
  }
};

#endif // GENERIC_TENTATIVE_HPP
