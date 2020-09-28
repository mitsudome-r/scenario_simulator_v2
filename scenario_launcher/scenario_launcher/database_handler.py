#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright 2020 Tier IV, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from scenario_common.logger import Logger
from scenario_common.manager import Manager
import pathlib


class DatabaseHandler():

    DATABASE_PATH = "config/scenario_database.yaml"

    @staticmethod
    def read_database():
        Logger.print_separator("scenario database")
        launcher_package_path = pathlib.Path(__file__).resolve().parent.parent
        print("package path: " + str(launcher_package_path))
        database_path = launcher_package_path / DatabaseHandler.DATABASE_PATH
        database = Manager.read_data(database_path)
        log_path = str(launcher_package_path / database["Log"])
        scenario_path = []
        for scenario in database["Scenario"]:
            scenario = str(launcher_package_path / scenario)
            Manager.path_checker(scenario)
            scenario_path.append(scenario)
        map_path = dict()
        map_database = database["Map"][0]
        for key, map in map_database.items():
            map = str(launcher_package_path / map)
            Manager.path_checker(map)
            map_path[key] = map
        return log_path, scenario_path, map_path


def main():
    log_path, scenario_path, map_path = DatabaseHandler.read_database()


if __name__ == "__main__":
    main()