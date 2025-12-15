add_library(uni_hal_example_stm32h7_assets STATIC)

uni_hal_add_file(uni_hal_example_stm32h7_assets bootstrap.min.css       "ox_asset_bootstrap"     "g_ox_asset_bootstrap"    )
uni_hal_add_file(uni_hal_example_stm32h7_assets bootstrap.bundle.min.js "ox_asset_bootstrap_js"  "g_ox_asset_bootstrap_js" )

uni_hal_add_file(uni_hal_example_stm32h7_assets fontawesome.min.css     "ox_asset_fontawesome_css"  "g_ox_asset_fontawesome_css" )
uni_hal_add_file(uni_hal_example_stm32h7_assets fa-solid-900.woff2      "ox_asset_fontawesome_wf"  "g_ox_asset_fontawesome_wf" )

uni_hal_add_file(uni_hal_example_stm32h7_assets dashboard.html          "ox_asset_dashboard"     "g_ox_asset_dashboard"    )

uni_hal_add_file(uni_hal_example_stm32h7_assets favicon.ico             "ox_asset_favicon"       "g_ox_asset_favicon"      )

uni_hal_add_file(uni_hal_example_stm32h7_assets index.html              "ox_asset_index"         "g_ox_asset_index"        )
uni_hal_add_file(uni_hal_example_stm32h7_assets updater.html            "ox_asset_updater"       "g_ox_asset_updater"      )

target_include_directories(uni_hal_example_stm32h7_assets PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
