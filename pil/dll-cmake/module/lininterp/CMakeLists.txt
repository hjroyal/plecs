

target_include_directories(
    lininterp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

target_link_libraries(
    lininterp
    PRIVATE
#    UtilsLibrary
#    logger_service
)

# if( ${TARGET_PLATFORM} STREQUAL "ARM_CORTEX" )
#     target_compile_definitions(gpio PUBLIC USE_DEVICE_SPECIFIC)
#     target_link_libraries(gpio PRIVATE NordicSDK::Common )
# endif()
