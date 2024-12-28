cmake_minimum_required(VERSION 3.5)

# Chỉ định thư mục chứa components


include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS components)

project(AutomaticPlantWateringSystem)
