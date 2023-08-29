if (NOT TARGET dfplayer)
    add_library(dfplayer INTERFACE)

    target_sources(dfplayer INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/dfplayer.c
    )

    target_include_directories(dfplayer INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries(dfplayer INTERFACE
        pico_stdlib
    )
endif()