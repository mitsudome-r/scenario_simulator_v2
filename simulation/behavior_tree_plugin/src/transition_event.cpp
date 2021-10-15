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

#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>

#include <behavior_tree_plugin/transition_event.hpp>

namespace behavior_tree_plugin
{
TransitionEvent::TransitionEvent(const std::shared_ptr<BT::Tree> & tree_ptr) : tree_ptr_(tree_ptr)
{
  first_timestamp_ = std::chrono::high_resolution_clock::now();
  auto subscribeCallback = [this](
                             BT::TimePoint timestamp, const BT::TreeNode & node,
                             BT::NodeStatus prev, BT::NodeStatus status) {
    if (status != BT::NodeStatus::IDLE) {
      if (type_ == BT::TimestampType::ABSOLUTE) {
        this->callback(timestamp.time_since_epoch(), node, prev, status);
      } else {
        this->callback(timestamp - first_timestamp_, node, prev, status);
      }
    }
  };
  auto visitor = [this, subscribeCallback](BT::TreeNode * node) {
    subscribers_.push_back(node->subscribeToStatusChange(std::move(subscribeCallback)));
  };
  BT::applyRecursiveVisitor(tree_ptr_->rootNode(), visitor);
}
}  // namespace behavior_tree_plugin
