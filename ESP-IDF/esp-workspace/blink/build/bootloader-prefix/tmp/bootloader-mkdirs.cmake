# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Microcontroller/ESP32/Espressif/frameworks/esp-idf-v5.4/components/bootloader/subproject")
  file(MAKE_DIRECTORY "D:/Microcontroller/ESP32/Espressif/frameworks/esp-idf-v5.4/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader"
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix"
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/tmp"
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/src"
  "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Microcontroller/ESP32/ESP-IDF/esp-workspace/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
