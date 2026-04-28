// Copyright 2015 TIER IV, Inc. All rights reserved.
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

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <cstdlib>
#include <openscenario_interpreter/reader/attribute.hpp>
#include <openscenario_interpreter/scope.hpp>
#include <scenario_simulator_exception/exception.hpp>
#include <string>

namespace
{
using openscenario_interpreter::substitute;

// Shared fixture for `substitute()` tests. Provides a default-constructed
// scope and an env-var setup/teardown helper. Constants here are reused by
// multiple substitution kinds; add new helpers for `$(var ...)`,
// `$(dirname)`, etc. as those tests are introduced.
class SubstituteTest : public ::testing::Test
{
protected:
  // The substitutions exercised here (`env`, `find-pkg-share`, plain text,
  // unknown) do not access the scope, so a null OpenScenario pointer is
  // sufficient. Tests that exercise `$(var ...)` or `$(dirname)` will need
  // to populate the scope or construct it from a real OpenScenario object.
  openscenario_interpreter::Scope scope{nullptr};

  static constexpr const char * kEnvVar = "OPENSCENARIO_INTERPRETER_TEST_ENV_VAR";
  static constexpr const char * kUnsetVar = "OPENSCENARIO_INTERPRETER_TEST_UNSET_VAR";

  void SetUp() override
  {
    ::unsetenv(kEnvVar);
    ::unsetenv(kUnsetVar);
  }

  void TearDown() override
  {
    ::unsetenv(kEnvVar);
    ::unsetenv(kUnsetVar);
  }
};
}  // namespace

// -----------------------------------------------------------------------------
// Plain text (no substitution token)
// -----------------------------------------------------------------------------

TEST_F(SubstituteTest, PlainStringIsReturnedUnchanged)
{
  EXPECT_EQ(substitute("plain string with no substitution", scope),
            "plain string with no substitution");
  EXPECT_EQ(substitute("", scope), "");
  EXPECT_EQ(substitute("/absolute/path/file.osm", scope), "/absolute/path/file.osm");
}

// -----------------------------------------------------------------------------
// $(env <name> [<default>])
// -----------------------------------------------------------------------------

TEST_F(SubstituteTest, EnvResolvesSetVariable)
{
  ::setenv(kEnvVar, "/some/path", 1);
  EXPECT_EQ(substitute(std::string("$(env ") + kEnvVar + ")", scope), "/some/path");
}

TEST_F(SubstituteTest, EnvConcatenatesWithSurroundingText)
{
  ::setenv(kEnvVar, "/some/path", 1);
  EXPECT_EQ(
    substitute(std::string("prefix-$(env ") + kEnvVar + ")/suffix", scope),
    "prefix-/some/path/suffix");
}

TEST_F(SubstituteTest, EnvPrefersSetValueOverDefault)
{
  ::setenv(kEnvVar, "/real/value", 1);
  EXPECT_EQ(
    substitute(std::string("$(env ") + kEnvVar + " /default/value)", scope),
    "/real/value");
}

TEST_F(SubstituteTest, EnvFallsBackToDefaultWhenUnset)
{
  EXPECT_EQ(
    substitute(std::string("$(env ") + kUnsetVar + " /default/value)", scope),
    "/default/value");
}

TEST_F(SubstituteTest, EnvDefaultPreservesInternalWhitespace)
{
  EXPECT_EQ(
    substitute(std::string("$(env ") + kUnsetVar + " path with spaces)", scope),
    "path with spaces");
}

TEST_F(SubstituteTest, EnvEmptyDefaultYieldsEmptyString)
{
  EXPECT_EQ(substitute(std::string("$(env ") + kUnsetVar + " )", scope), "");
}

TEST_F(SubstituteTest, EnvThrowsWhenUnsetAndNoDefault)
{
  EXPECT_THROW(
    substitute(std::string("$(env ") + kUnsetVar + ")", scope),
    common::SyntaxError);
}

TEST_F(SubstituteTest, EnvResolvesEmptyValueWhenSetToEmptyString)
{
  ::setenv(kEnvVar, "", 1);
  EXPECT_EQ(
    substitute(std::string("$(env ") + kEnvVar + " /default/value)", scope), "");
}

// -----------------------------------------------------------------------------
// $(find-pkg-share <package>)
// -----------------------------------------------------------------------------

TEST_F(SubstituteTest, FindPkgShareResolvesKnownPackage)
{
  // `openscenario_interpreter` is this package itself, so its share directory
  // is guaranteed to exist whenever the test binary runs from an installed
  // workspace.
  const auto expected =
    ament_index_cpp::get_package_share_directory("openscenario_interpreter");
  EXPECT_EQ(substitute("$(find-pkg-share openscenario_interpreter)", scope), expected);
  EXPECT_EQ(
    substitute("$(find-pkg-share openscenario_interpreter)/features", scope),
    expected + "/features");
}

// -----------------------------------------------------------------------------
// Unknown substitution
// -----------------------------------------------------------------------------

TEST_F(SubstituteTest, UnknownSubstitutionThrows)
{
  EXPECT_THROW(substitute("$(no-such-substitution arg)", scope), common::SyntaxError);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
