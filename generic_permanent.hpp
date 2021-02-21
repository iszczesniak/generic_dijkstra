#ifndef GENERIC_PERMANENT_HPP
#define GENERIC_PERMANENT_HPP

#include <utility>
#include <vector>

// The container type for storing permanent generic labels.  A vertex
// can have many labels or none, so we store them in a container.
template <typename Label>
struct generic_permanent:
  std::vector<std::vector<Label>>
{
  // The label type
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::vector<label_type>;
  // The type of the vector of vertex data.
  using base = std::vector<vd_type>;
  // The size type of the base.
  using size_type = typename base::size_type;

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

#endif // GENERIC_PERMANENT_HPP
