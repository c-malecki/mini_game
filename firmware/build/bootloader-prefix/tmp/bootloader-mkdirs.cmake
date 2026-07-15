# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/chris/.espressif/v6.0.1/esp-idf/components/bootloader/subproject"
  "/home/chris/Projects/mini_game/firmware/build/bootloader"
  "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix"
  "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/tmp"
  "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/src/bootloader-stamp"
  "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/src"
  "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/chris/Projects/mini_game/firmware/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
