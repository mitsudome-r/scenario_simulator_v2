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

#include <gtest/gtest.h>

#include <simulation_api/metrics/metrics_manager.hpp>

#include <memory>

class TestMetrics : metrics::MetricsBase
{
public:
  TestMetrics()
  : metrics::MetricsBase("ego", "test") {}
  void calculate()
  {
    foundSpecificationViolation("error");
  }
};

TEST(Metrics, AddMetrics)
{
  metrics::MetricsManager manager;
  // manager.addMetrics();
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
