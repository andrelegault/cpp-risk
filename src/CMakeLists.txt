file(GLOB_RECURSE HEADERS
    "*.h"
)

file(GLOB_RECURSE SOURCES
    "*.cpp"
)

add_library(WarZone STATIC ${HEADERS} ${SOURCES})
target_include_directories(WarZone PUBLIC map)