#ifndef GENERIC_LABEL_CREATOR_HPP
#define GENERIC_LABEL_CREATOR_HPP

#include "graph.hpp"

#include <list>

template <typename Label>
class generic_label_creator
{
  using label_type = Label;
  
public:
  template <typename Edge>
  std::list<label_type>
  operator()(const label_type &l, const Edge &e) const
  {
    // Candidate weight.
    auto c_weight = get_weight(l) + get_weight(e);

    // The label resources.
    const auto &l_resources = get_resources(l);
    // The edge resources.
    const auto &e_resources = get_resources(e);
    // The candidate resources.
    auto c_resources = intersection(l_resources, e_resources);

    std::list<label_type> result;

    for (auto &cr: c_resources)
      // The candidate label.
      result.emplace_back(e, c_weight, cr);

    return result;
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
