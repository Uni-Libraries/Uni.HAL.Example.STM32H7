add_library(corejson STATIC)
target_sources(corejson PRIVATE src_corejson/source/core_json.c)
target_include_directories(corejson PUBLIC src_corejson/source/include)
