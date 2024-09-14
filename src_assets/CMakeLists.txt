add_library(uni_hal_example_stm32h7_lib STATIC)

uni_hal_add_file(uni_hal_example_stm32h7_lib bootstrap.min.css       "ox_asset_bootstrap"     "g_ox_asset_bootstrap"    )
uni_hal_add_file(uni_hal_example_stm32h7_lib bootstrap.bundle.min.js "ox_asset_bootstrap_js"  "g_ox_asset_bootstrap_js" )

uni_hal_add_file(uni_hal_example_stm32h7_lib fontawesome.min.css     "ox_asset_fontawesome_css"  "g_ox_asset_fontawesome_css" )
uni_hal_add_file(uni_hal_example_stm32h7_lib fa-solid-900.woff2      "ox_asset_fontawesome_wf"  "g_ox_asset_fontawesome_wf" )

uni_hal_add_file(uni_hal_example_stm32h7_lib favicon.ico             "ox_asset_favicon"       "g_ox_asset_favicon"      )

uni_hal_add_file(uni_hal_example_stm32h7_lib index.html              "ox_asset_index"         "g_ox_asset_index"        )
uni_hal_add_file(uni_hal_example_stm32h7_lib updater.html            "ox_asset_updater"       "g_ox_asset_updater"      )

target_include_directories(uni_hal_example_stm32h7_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
