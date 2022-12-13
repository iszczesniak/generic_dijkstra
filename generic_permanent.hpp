#ifndef GENERIC_PERMANENT_HPP
#define GENERIC_PERMANENT_HPP

#include <utility>
#include <vector>

// The container type for storing permanent generic labels.  All
// labels for a given index are incomparable.  An index can have many
// labels or none, so we store them in a container.
template <typename Label>
struct generic_permanent: std::vector<std::vector<Label>>
{
  // The label type.
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::vector<label_type>;
  // The type of the vector of vertex data.
  using base_type = std::vector<vd_type>;
  // The size type of the base type.
  using size_type = typename base_type::size_type;

  generic_permanent(size_type count): base_type(count)
  {
  }

  // Pushes a new label, and returns a reference to it.
  template <typename T>
  const label_type &
  push(T &&l)
  {
    // The index of the target vertex of the label.
    const auto &ti = get_index(l);
    // Push the label back.
    base_type::operator[](ti).push_back(std::forward<T>(l));

    return base_type::operator[](ti).back();
  }
};

/**
 * Is there in P a label that is better than or equal to label j?
 */
template <typename Label>
bool
has_better_or_equal(const generic_permanent<Label> &P, const Label &j)
{
  return boe(P[get_index(j)], j);
}

#endif // GENERIC_PERMANENT_HPP
