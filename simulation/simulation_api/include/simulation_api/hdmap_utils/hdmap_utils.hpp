// Copyright 2015-2020 Autoware Foundation. All rights reserved.
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

#ifndef SIMULATION_API__HDMAP_UTILS__HDMAP_UTILS_HPP_
#define SIMULATION_API__HDMAP_UTILS__HDMAP_UTILS_HPP_

#include <simulation_api/entity/entity_status.hpp>
#include <simulation_api/math/hermite_curve.hpp>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <autoware_auto_msgs/msg/had_map_bin.hpp>
#include <geometry_msgs/msg/vector3.h>
#include <geographic_msgs/msg/geo_point.hpp>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <lanelet2_core/LaneletMap.h>
#include <lanelet2_core/geometry/Lanelet.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/primitives/LaneletSequence.h>
#include <lanelet2_extension/utility/message_conversion.hpp>
#include <lanelet2_extension/utility/utilities.hpp>
#include <lanelet2_routing/Route.h>
#include <lanelet2_routing/RoutingCost.h>
#include <lanelet2_routing/RoutingGraph.h>
#include <lanelet2_routing/RoutingGraphContainer.h>
#include <lanelet2_traffic_rules/TrafficRulesFactory.h>
#include <visualization_msgs/msg/marker_array.hpp>
#include <boost/optional.hpp>

#include <map>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <memory>


namespace hdmap_utils
{
class HdMapError : public std::runtime_error
{
public:
  explicit HdMapError(const char * message)
  : runtime_error(message) {}
};

class HdMapUtils
{
public:
  explicit HdMapUtils(std::string lanelet_path, geographic_msgs::msg::GeoPoint origin);
  const autoware_auto_msgs::msg::HADMapBin toMapBin();
  void insertMarkerArray(
    visualization_msgs::msg::MarkerArray & a1,
    const visualization_msgs::msg::MarkerArray & a2) const;
  std::vector<geometry_msgs::msg::Point> toMapPoints(
    std::int64_t lanelet_id,
    std::vector<double> s);
  boost::optional<geometry_msgs::msg::PoseStamped> toMapPose(
    std::int64_t lanelet_id, double s,
    double offset,
    geometry_msgs::msg::Quaternion quat);
  boost::optional<geometry_msgs::msg::PoseStamped> toMapPose(
    std::int64_t lanelet_id, double s,
    double offset,
    geometry_msgs::msg::Vector3 rpy);
  boost::optional<geometry_msgs::msg::PoseStamped> toMapPose(
    std::int64_t lanelet_id, double s,
    double offset);
  boost::optional<geometry_msgs::msg::PoseStamped> toMapPose(
    simulation_api::entity::EntityStatus status);
  std::vector<std::int64_t> getNextLaneletIds(std::int64_t lanelet_id, std::string turn_direction);
  std::vector<std::int64_t> getNextLaneletIds(std::int64_t lanelet_id) const;
  std::vector<std::int64_t> getPreviousLaneletIds(std::int64_t lanelet_id) const;
  boost::optional<int> getLaneChangeableLenletId(std::int64_t lanelet_id, std::string direction);
  boost::optional<double> getDistanceToStopLine(
    std::vector<std::int64_t> following_lanelets, std::int64_t lanelet_id,
    double s);
  double getLaneletLength(std::int64_t lanelet_id) const;
  bool isInLanelet(std::int64_t lanelet_id, double s);
  boost::optional<double> getLongitudinalDistance(
    std::int64_t from_lanelet_id, double from_s,
    std::int64_t to_lanelet_id, double to_s);
  double getSpeedLimit(std::vector<std::int64_t> lanelet_ids);
  std::vector<std::int64_t> getFollowingLanelets(std::int64_t lanelet_id, double distance = 100);
  std::vector<geometry_msgs::msg::Point> getCenterPoints(std::int64_t lanelet_id);
  std::vector<geometry_msgs::msg::Point> clipTrajectoryFromLaneletIds(
    std::int64_t lanelet_id, double s,
    std::vector<std::int64_t> lanelet_ids, double foward_distance = 20);
  bool canChangeLane(std::int64_t from_lanelet_id, std::int64_t to_lanelet_id);
  boost::optional<std::pair<simulation_api::math::HermiteCurve,
    double>> getLaneChangeTrajectory(
    geometry_msgs::msg::Pose from_pose,
    std::int64_t to_lanelet_id);
  boost::optional<simulation_api::math::HermiteCurve> getLaneChangeTrajectory(
    geometry_msgs::msg::Pose from_pose,
    std::int64_t to_lanelet_id, double to_s, double tangent_vector_size = 100);
  boost::optional<geometry_msgs::msg::Vector3> getTangentVector(std::int64_t lanelet_id, double s);
  std::vector<std::int64_t> getRoute(std::int64_t from_lanelet_id, std::int64_t to_lanelet_id);
  std::vector<std::int64_t> getConflictingCrosswalkIds(std::vector<std::int64_t> lanelet_ids) const;
  boost::optional<double> getCollisionPointInLaneCoordinate(
    std::int64_t lanelet_id,
    int crossing_lanelet_id);
  const visualization_msgs::msg::MarkerArray generateMarker() const;
  const std::vector<std::int64_t> getRightOfWayLaneletIds(std::int64_t lanelet_id) const;
  const std::unordered_map<int, std::vector<std::int64_t>> getRightOfWayLaneletIds(
    std::vector<std::int64_t> lanelet_ids) const;

private:
  std::vector<std::shared_ptr<const lanelet::TrafficSign>> getTrafficSignRegElementsOnPath(
    std::vector<std::int64_t> lanelet_ids);
  std::vector<lanelet::ConstLineString3d> getStopLinesOnPath(std::vector<std::int64_t> lanelet_ids);
  geometry_msgs::msg::Vector3 getVectorFromPose(geometry_msgs::msg::Pose pose, double magnitude);
  void mapCallback(const autoware_auto_msgs::msg::HADMapBin & msg);
  lanelet::LaneletMapPtr lanelet_map_ptr_;
  lanelet::routing::RoutingGraphConstPtr vehicle_routing_graph_ptr_;
  lanelet::traffic_rules::TrafficRulesPtr traffic_rules_vehicle_ptr_;
  lanelet::routing::RoutingGraphConstPtr pedestrian_routing_graph_ptr_;
  lanelet::traffic_rules::TrafficRulesPtr traffic_rules_pedestrian_ptr_;
  lanelet::routing::RoutingGraphContainerUPtr overall_graphs_ptr_;
  double getTrajectoryLength(std::vector<geometry_msgs::msg::Point> trajectory);
  std::vector<double> calcEuclidDist(
    const std::vector<double> & x, const std::vector<double> & y,
    const std::vector<double> & z);
  void overwriteLaneletsCenterline();
  lanelet::LineString3d generateFineCenterline(
    const lanelet::ConstLanelet & lanelet_obj, const double resolution);
  std::vector<lanelet::BasicPoint3d> resamplePoints(
    const lanelet::ConstLineString3d & line_string, const int32_t num_segments);
  std::pair<size_t, size_t> findNearestIndexPair(
    const std::vector<double> & accumulated_lengths, const double target_length);
  std::vector<double> calculateAccumulatedLengths(const lanelet::ConstLineString3d & line_string);
  std::vector<double> calculateSegmentDistances(const lanelet::ConstLineString3d & line_string);
};
}  // namespace hdmap_utils

#endif  // SIMULATION_API__HDMAP_UTILS__HDMAP_UTILS_HPP_
