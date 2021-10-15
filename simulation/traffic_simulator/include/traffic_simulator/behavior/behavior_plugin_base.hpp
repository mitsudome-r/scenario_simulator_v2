// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
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

#ifndef TRAFFIC_SIMULATOR__BEHAVIOR__BEHAVIOR_PLUGIN_BASE_HPP_
#define TRAFFIC_SIMULATOR__BEHAVIOR__BEHAVIOR_PLUGIN_BASE_HPP_

#include <boost/optional.hpp>
#include <openscenario_msgs/msg/driver_model.hpp>
#include <openscenario_msgs/msg/entity_status.hpp>
#include <openscenario_msgs/msg/entity_type.hpp>
#include <openscenario_msgs/msg/obstacle.hpp>
#include <openscenario_msgs/msg/pedestrian_parameters.hpp>
#include <openscenario_msgs/msg/vehicle_parameters.hpp>
#include <openscenario_msgs/msg/waypoints_array.hpp>
#include <string>
#include <traffic_simulator/hdmap_utils/hdmap_utils.hpp>
#include <traffic_simulator/traffic_lights/traffic_light_manager.hpp>
#include <unordered_map>

namespace entity_behavior
{
class BehaviorPluginBase
{
private:
  std::string current_action_;

public:
  virtual void configure() = 0;
  virtual void update(double current_time, double step_time) = 0;
  const std::string getCurrentAction() const { return current_action_; }

  typedef std::unordered_map<std::string, openscenario_msgs::msg::EntityType> EntityTypeDict;
  typedef std::unordered_map<std::string, openscenario_msgs::msg::EntityStatus> EntityStatusDict;

#define DEFINE_GETTER_SETTER(NAME, KEY, TYPE)     \
  virtual TYPE get##NAME() = 0;                   \
  virtual void set##NAME(const TYPE & value) = 0; \
  const std::string get##NAME##Key() const { return KEY; };

  // clang-format off
  DEFINE_GETTER_SETTER(CurrentTime, "current_time", double)
  DEFINE_GETTER_SETTER(DriverModel, "driver_model", openscenario_msgs::msg::DriverModel)
  DEFINE_GETTER_SETTER(EntityStatus, "entity_status", openscenario_msgs::msg::EntityStatus)
  DEFINE_GETTER_SETTER(EntityTypeList, "entity_type_list", EntityTypeDict)
  DEFINE_GETTER_SETTER(HdMapUtils, "hdmap_utils", std::shared_ptr<hdmap_utils::HdMapUtils>)
  DEFINE_GETTER_SETTER(Obstacle, "obstacle", boost::optional<openscenario_msgs::msg::Obstacle>)
  DEFINE_GETTER_SETTER(OtherEntityStatus, "other_entity_status", EntityStatusDict)
  DEFINE_GETTER_SETTER(PedestrianParameters, "pedestrian_parameters", openscenario_msgs::msg::PedestrianParameters)
  DEFINE_GETTER_SETTER(Request, "request", std::string)
  DEFINE_GETTER_SETTER(RouteLanelets, "route_lanelets", std::vector<std::int64_t>)
  DEFINE_GETTER_SETTER(StepTime, "step_time", double)
  DEFINE_GETTER_SETTER(TargetSpeed, "target_speed", boost::optional<double>)
  DEFINE_GETTER_SETTER(ToLaneletId, "to_lanelet_id", std::int64_t)
  DEFINE_GETTER_SETTER(TrafficLightManager, "traffic_light_manager",std::shared_ptr<traffic_simulator::TrafficLightManager>)
  DEFINE_GETTER_SETTER(UpdatedStatus, "updated_status", openscenario_msgs::msg::EntityStatus)
  DEFINE_GETTER_SETTER(VehicleParameters, "vehicle_parameters", openscenario_msgs::msg::VehicleParameters)
  DEFINE_GETTER_SETTER(Waypoints, "waypoints", openscenario_msgs::msg::WaypointsArray)
  // clang-format on
#undef DEFINE_GETTER_SETTER
};
}  // namespace entity_behavior

#endif  // TRAFFIC_SIMULATOR__BEHAVIOR__BEHAVIOR_PLUGIN_BASE_HPP_