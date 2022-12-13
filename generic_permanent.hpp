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
  // We could go for the easy implementation: iterate for each label
  // i, and compare it to label j.  But we take advantage of the fact
  // that the elements in the vector are sorted with operator <.  The
  // < operator establishes order between incomparable elements: label
  // i < label i+1.  The stored elements should be incomparable with
  // each other.  We start with the first label as it's the most
  // likely to be better than or equal to j.
  for (const auto &i: P[get_index(j)])
    {
      // Stop searching when we reach label i such that j < i, because
      // then label i cannot be better than or equal to j; label j can
      // be better than or incomparable with i.  The same applies to
      // the labels that follow i, because the permanent labels are
      // sorted with <, and < is transitive.  Therefore we can break
      // the loop.
      if (j < i)
        break;

      // Here we know that i <= j, but labels i and j can be
      // incomparable.  Therefore we need to check whether label i
      // better than or equal to label j.
      if (boe(i, j))
        return true;
    }

  return false;
}

#endif // GENERIC_PERMANENT_HPP
