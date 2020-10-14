// Copyright 2015-2019 Autoware Foundation. All rights reserved.
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

#ifndef JUNIT_EXPORTER__JUNIT_EXPORTER_HPP_
#define JUNIT_EXPORTER__JUNIT_EXPORTER_HPP_

#include <junit_exporter/test_suites.hpp>

#include <pugixml.hpp>
#include <vector>
#include <string>

namespace junit_exporter
{
class JunitExporter
{
public:
  JunitExporter();
  void write(const std::string & path);
  void addTestCase(
    const std::string & name,
    const std::string & test_suite,
    const double & time,
    const TestResult & result);
  void addTestCase(
    const std::string & name,
    const std::string & test_suite,
    const double & time,
    const TestResult & result,
    const std::string & type,
    const std::string & description);

private:
  pugi::xml_document doc_;
  TestSuites test_suites_;
};
}  // namespace junit_exporter

#endif  // JUNIT_EXPORTER__JUNIT_EXPORTER_HPP_