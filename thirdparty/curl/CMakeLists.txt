set(INCLUDE_CURL "${CMAKE_CURRENT_SOURCE_DIR}/include"  CACHE INTERNAL "")
set(LIB_CURL "${CMAKE_CURRENT_SOURCE_DIR}/lib" CACHE INTERNAL "")

if(NOT TARGET Thirdparty::Curl)
    add_library(Thirdparty::Curl UNKNOWN IMPORTED GLOBAL)
	set(CURL_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/Win/Release/libcurl_imp.lib")
    set_target_properties(Thirdparty::Curl PROPERTIES
    IMPORTED_LOCATION "${CURL_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
endif()

