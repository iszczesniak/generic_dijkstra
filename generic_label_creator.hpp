#ifndef GENERIC_LABEL_CREATOR_HPP
#define GENERIC_LABEL_CREATOR_HPP

#include "adaptive_units.hpp"
#include "generic_label.hpp"
#include "graph.hpp"
#include "units.hpp"

#include <list>
#include <optional>

template <typename Label>
auto
get_cost(const Label &);

template <typename Label>
auto
get_units(const Label &);

template <typename Graph, typename Cost, typename Units>
class generic_label_creator
{
  using Label = generic_label<Graph, Cost, Units>;

  // The graph.
  const Graph &m_g;

public:
  generic_label_creator(const Graph &g):
    m_g(g)
  {
  }

  //template <Callable<Cost, SU> F = EmptyCallable<Cost, SU>>
  template <Callable<Cost, SU> F>
  std::list<Label>
  operator()(const Edge<Graph> &e, const Label &l, F f = {}) const
  {
    // The cost of the edge.
    Cost ec = boost::get(boost::edge_weight, m_g, e);
    // Candidate cost.
    Cost c_c = get_cost(l) + ec;

    // The label units.
    const Units &l_units = get_units(l);
    // The units available on the edge.
    const auto &e_su = boost::get(boost::edge_su, m_g, e);
    // The candidate SU: the su of label l that can be carried by
    // edge e.
    auto c_su = intersection(SU{l_units}, e_su);

    // Filter the SU if needed.
    f(c_c, c_su);

    std::list<Label> ls;

    for (auto &cu: c_su)
      // The candidate label.
      ls.push_back(Label(c_c, std::move(cu), e, target(e, m_g)));

    return ls;
  }
};

#endif // GENERIC_LABEL_CREATOR_HPP
