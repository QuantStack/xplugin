add_library(testlib_01 SHARED testlib_01.cpp)
add_dependencies(testlib_01 all_plugins)

set_target_properties(testlib_01 PROPERTIES PREFIX "")
set_target_properties(testlib_01 PROPERTIES OUTPUT_NAME "testlib_01")
set_target_properties(testlib_01 PROPERTIES SUFFIX ".xplugin")
