#ifndef GENERAL_LABEL_CREATOR_HPP
#define GENERAL_LABEL_CREATOR_HPP

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
    // Candidate cost.
    Cost c_c = get_weight(l) + get_weight(e);

    // The label resources.
    const auto &l_resources = get_resources(l);
    // The resources available on the edge.
    const auto &e_resources = get_resources(e);
    // The candidate resources.
    auto c_resources = intersection(l_resources, e_resources);

    std::list<label_type> l;

    for (auto &cr: c_resources)
      // The candidate label.
      l.emplace_back(c_c, std::move(cu), e);

    return l;
  }
};

#endif // GENERAL_LABEL_CREATOR_HPP
