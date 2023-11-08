#ifndef GENERIC_LABEL_CREATOR_HPP
#define GENERIC_LABEL_CREATOR_HPP

#include <utility>

struct generic_label_creator
{
  template <typename Label, typename Edge>

  auto
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

    return std::make_pair(c_c, std::move(c_resources));
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
