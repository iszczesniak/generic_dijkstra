#ifndef GENERIC_PERMANENT_HPP
#define GENERIC_PERMANENT_HPP

#include "generic_label.hpp"

#include <utility>
#include <vector>

// The container type for storing permanent generic labels.  A vertex
// can have many labels or none, so we store them in a container.
template <typename Graph, typename Cost, typename Units>
struct generic_permanent:
  std::vector<std::vector<generic_label<Cost, Units, Edge<Graph>>>>
{
  // That's the label type we're using.
  using label_t = generic_label<Cost, Units, Edge<Graph>>;
  // The type of data a vertex has.
  using vd_t = std::vector<label_t>;
  // The type of the vector of vertex data.
  using base = std::vector<vd_t>;

  // The size type of the base.
  using size_type = typename base::size_type;

  generic_permanent(size_type count): base(count)
  {
  }

  // Pushes a new label, and returns a reference to it.
  template <typename T>
  const label_t &
  push(T &&l)
  {
    // The key of the target vertex of the label.
    const auto &tk = key(get_target(l));
    // Push the label back.
    base::operator[](tk).push_back(std::forward<T>(l));

    return base::operator[](tk).back();
  }
};

#endif // GENERIC_PERMANENT_HPP
