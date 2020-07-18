#ifndef GENERIC_LABEL_CREATOR_HPP
#define GENERIC_LABEL_CREATOR_HPP

#include "generic_label.hpp"
#include "units.hpp"

#include <list>
#include <optional>

template <typename Cost, typename Units, typename Edge>
class generic_label_creator
{
  using Label = generic_label<Cost, Units, Edge>;

public:
  template <Callable<Cost, SU &> F = EmptyCallable<Cost, SU &>>
  std::list<Label>
  operator()(const Edge &e, const Label &l, F f = {}) const
  {
    // The cost of the edge.
    Cost ec = cost(e);
    // Candidate cost.
    Cost c_c = cost(l) + ec;

    // The label units.
    const Units &l_units = units(l);
    // The units available on the edge.
    const auto &e_su = units(e);
    // The candidate SU: the su of label l that can be carried by
    // edge e.
    auto c_su = intersection(SU{l_units}, e_su);

    // Filter the SU if needed.
    f(c_c, c_su);

    std::list<Label> ls;

    for (auto &cu: c_su)
      // The candidate label.
      ls.push_back(Label(c_c, std::move(cu), e));

    return ls;
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
