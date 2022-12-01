#ifndef GENERIC_LABEL_CREATOR_HPP
#define GENERIC_LABEL_CREATOR_HPP

#include <list>

struct generic_label_creator
{
  template <typename Label, typename Edge>
  std::list<Label>
  operator()(const Label &l, const Edge &e) const
  {
    // Candidate weight.
    auto c_weight = get_weight(l) + get_weight(e);

    // The label resources.
    const auto &l_resources = get_resources(l);
    // The edge resources.
    const auto &e_resources = get_resources(e);
    // The candidate resources.
    auto c_resources = intersection(l_resources, e_resources);

    std::list<Label> result;

    for (auto &cr: c_resources)
      // The candidate label.
      result.emplace_back(e, c_weight, cr);

    return result;
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
