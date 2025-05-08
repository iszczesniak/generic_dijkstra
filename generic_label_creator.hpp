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
    auto c_w = get_weight(l) + get_weight(e);

    // The label resources.
    const auto &l_r = get_resources(l);
    // The edge resources.
    const auto &e_r = get_resources(e);
    // The candidate resources.
    auto c_r = intersection(l_r, e_r);

    return std::make_pair(c_w, std::move(c_r));
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
