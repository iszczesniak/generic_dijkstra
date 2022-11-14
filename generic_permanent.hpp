#ifndef GENERIC_PERMANENT_HPP
#define GENERIC_PERMANENT_HPP

#include "graph_interface.hpp"
#include "label_robe.hpp"

#include <utility>
#include <vector>

// The container type for storing permanent generic labels.  A vertex
// can have many labels or none, so we store them in a container.
template <typename Label, typename Edge>
struct generic_permanent:
  std::vector<std::vector<label_robe<Label, Edge>>>
{
  // The label type.
  using label_type = Label;
  // The robe type.
  using robe_type = label_robe<Label, Edge>;
  // The type of data a vertex has.
  using vd_type = std::vector<robe_type>;
  // The type of the vector of vertex data.
  using base = std::vector<vd_type>;
  // The size type of the base.
  using size_type = typename base::size_type;
  // The index type of the vertex.
  using index_type = Index<Vertex<Edge>>;

  generic_permanent(size_type count): base(count)
  {
  }

  // Pushes a new label, and returns a reference to it.
  template <typename T>
  const label_type &
  push(T &&l)
  {
    // The index of the target vertex of the label.
    const auto &ti = get_index(get_target(l));
    // Push the label back.
    base::operator[](ti).push_back(std::forward<T>(l));

    return base::operator[](ti).back();
  }
};

/**
 * Is there in P a label that is better than or equal to label j?
 */
template <typename Label, typename Edge>
bool
has_better_or_equal(const generic_permanent<Label, Edge> &P,
                    const label_robe<Label, Edge> &j)
{
  // We could go for the easy implementation where we iterate for each
  // label i, and compare it to label j.  But we take advantage of the
  // fact that the elements in the vector are sorted by weight first.
  for (const auto &i: P[get_index(get_target(j))])
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

#endif // GENERIC_PERMANENT_HPP
