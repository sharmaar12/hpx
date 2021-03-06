# Copyright (c) 2007-2014 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    no_output_1173
   )

set(no_output_1173_PARAMETERS LOCALITIES 2)
set(no_output_1173_FLAGS DEPENDENCIES iostreams_component)

foreach(test ${tests})
  set(sources
      ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${test}_test
                     SOURCES ${sources}
                     ${${test}_FLAGS}
                     FOLDER "Tests/Regressions/Iostreams")

  add_hpx_regression_test("iostreams" ${test} ${${test}_PARAMETERS})

  # add a custom target for this example
  add_hpx_pseudo_target(tests.regressions.iostreams.${test})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.regressions.iostreams
                              tests.regressions.iostreams.${test})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.regressions.iostreams.${test}
                              ${test}_test_exe)
endforeach()

