project(iservice2)

add_library(iservice2 STATIC
	iservice.c
	protocol_format.c
	protocol_driver.c
)


target_include_directories(iservice2
	PUBLIC ${base_SOURCE_DIR}
	PUBLIC ${log_SOURCE_DIR}
	PUBLIC ${common_SOURCE_DIR}
	PUBLIC ${KIT_ROOT_DIR}/conf
	PUBLIC ${KIT_ROOT_DIR}/lib
	PUBLIC ${KIT_ROOT_DIR}/drivers
)

target_link_libraries(iservice2
    PUBLIC log
	PUBLIC common
	PUBLIC drivers
	PUBLIC queue
)

# file(COPY ${PROJECT_SOURCE_DIR}/protocol_format.json
#     DESTINATION ${CMAKE_BINARY_DIR}/data/public/media
# )