import os
from glob import glob
from setuptools import setup

package_name = 'scenario_test_runner'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name), glob('launch/*.launch.py'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='tanaka-car',
    maintainer_email='ttatcoder@outlook.jp',
    description='scenario test runner package',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'scenario_test_runner = scenario_test_runner.scenario_test_runner:main',
            'database_handler = scenario_test_runner.database_handler:main',
            'lifecycle_controller = scenario_test_runner.lifecycle_controller:main'
        ],
    },
)