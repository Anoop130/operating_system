# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp-build"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/tmp"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp-stamp"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src"
  "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/anoop130/operating_system/10_power_cal_threads/build/gmp/src/gmp-stamp${cfgdir}") # cfgdir has leading slash
endif()
