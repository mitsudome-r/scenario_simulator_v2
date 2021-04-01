/**
 * @file traffic_controller.hpp
 * @author Masaya Kataoka (masaya.kataoka@tier4.jp)
 * @brief class definition for the traffic controller
 * @version 0.1
 * @date 2021-04-01
 *
 * @copyright Copyright(c) Tier IV.Inc {2015-2021}
 *
 */

// Copyright 2015-2020 TierIV.inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SIMULATION_API__TRAFFIC__TRAFFIC_CONTROLLER_HPP_
#define SIMULATION_API__TRAFFIC__TRAFFIC_CONTROLLER_HPP_

#include <simulation_api/hdmap_utils/hdmap_utils.hpp>
#include <simulation_api/traffic/traffic_module_base.hpp>

#include <memory>

namespace simulation_api
{
namespace traffic
{
class TrafficController
{
public:
  explicit TrafficController(
    std::shared_ptr<hdmap_utils::HdMapUtils> hdmap_utils,
    bool auto_sink = false);
  template<typename T, typename ... Ts>
  void addModule(const std::string & name, Ts && ... xs)
  {
    auto module_ptr = std::make_shared<T>(std::forward<Ts>(xs)...);
    modules_.emplace_back(module_ptr);
  }
  const bool auto_sink;
  void execute();

private:
  const std::shared_ptr<hdmap_utils::HdMapUtils> hdmap_utils_;
  std::vector<std::shared_ptr<simulation_api::traffic::TraffiModuleBase>> modules_;
};
}  // namespace traffic
}  // namespace simulation_api

#endif  // SIMULATION_API__TRAFFIC__TRAFFIC_CONTROLLER_HPP_
