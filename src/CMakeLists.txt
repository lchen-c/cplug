file(GLOB_RECURSE HEADER
	"*.h"
	"*.hpp"
	"file/*.h"
	"file/*.hpp"
	"log/*.h"
	"log/*.hpp"
	"thread/*.h"
	"thread/*.hpp"
	"mem/*.h"
	"mem/*.hpp"
	"json/*.h"
	"json/*.hpp"
	"aws/*.h"
	"aws/*.hpp"
	"compress/*.h"
	"compress/*.hpp"
	"sql/*.h"
	"sql/*.hpp"
	"mutex/*.h"
	"mutex/*.hpp"
	"encryption/*.h"
	"encryption/*.hpp"
)

file(GLOB_RECURSE SOURCE
	"*.c"
	"*.cpp"
	"file/*.c"
	"file/*.cpp"
	"log/*.c"
	"log/*.cpp"
	"thread/*.c"
	"thread/*.cpp"
	"mem/*.c"
	"mem/*.cpp"
	"json/*.c"
	"json/*.cpp"
	"aws/*.c"
	"aws/*.cpp"
	"compress/*.c"
	"compress/*.cpp"
	"sql/*.c"
	"sql/*.cpp"
	"mutex/*.c"
	"mutex/*.cpp"
	"encryption/*.c"
	"encryption/*.cpp"
)
	
source_group("Header" FILES ${HEADER})
source_group("Source" FILES  ${SOURCE})

include_directories(${INCLUDE_BOOST})
include_directories(${INCLUDE_AWS})

if (WIN32)
	include_directories(${INCLUDE_MYSQL}/Win)
	include_directories(${INCLUDE_COMPRESS}/Win)
else()
	include_directories(${INCLUDE_MYSQL}/Linux)
	include_directories(${INCLUDE_COMPRESS}/Linux)
endif()

if (WIN32)
	add_library(libcplug STATIC ${SOURCE} ${HEADER})
	target_compile_features(libcplug PUBLIC cxx_std_11)
else()
	add_library(cplug STATIC ${SOURCE} ${HEADER})
	target_compile_features(cplug PUBLIC cxx_std_11)
endif()

####################install include file############################
file(GLOB_RECURSE INSTALL_INCLUDE
	"cplug_*.h"
	"cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_FILE
	"file/cplug_*.h"
	"file/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_LOG
	"log/cplug_*.h"
	"log/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_THREAD
	"thread/cplug_*.h"
	"thread/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_MEM
	"mem/cplug_*.h"
	"mem/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_AWS
	"aws/cplug_*.h"
	"aws/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_COMPRESS
	"compress/cplug_*.h"
	"compress/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_SQL
	"sql/cplug_*.h"
	"sql/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_JSON
	"json/json.h"
)

file(GLOB_RECURSE INSTALL_MUTEX
	"mutex/cplug_*.h"
	"mutex/cplug_*.hpp"
)

file(GLOB_RECURSE INSTALL_ENCRYPTION
	"encryption/cplug_*.h"
	"encryption/cplug_*.hpp"
)

install(FILES ${INSTALL_INCLUDE} DESTINATION ${PROJECT_SOURCE_DIR}/include)
install(FILES ${INSTALL_FILE} DESTINATION ${PROJECT_SOURCE_DIR}/include/file)  
install(FILES ${INSTALL_LOG} DESTINATION ${PROJECT_SOURCE_DIR}/include/log)
install(FILES ${INSTALL_THREAD} DESTINATION ${PROJECT_SOURCE_DIR}/include/thread)
install(FILES ${INSTALL_JSON} DESTINATION ${PROJECT_SOURCE_DIR}/include/json)
install(FILES ${INSTALL_MEM} DESTINATION ${PROJECT_SOURCE_DIR}/include/mem)
install(FILES ${INSTALL_AWS} DESTINATION ${PROJECT_SOURCE_DIR}/include/aws) 
install(FILES ${INSTALL_COMPRESS} DESTINATION ${PROJECT_SOURCE_DIR}/include/compress) 
install(FILES ${INSTALL_SQL} DESTINATION ${PROJECT_SOURCE_DIR}/include/sql) 
install(FILES ${INSTALL_MUTEX} DESTINATION ${PROJECT_SOURCE_DIR}/include/mutex) 
install(FILES ${INSTALL_ENCRYPTION} DESTINATION ${PROJECT_SOURCE_DIR}/include/encryption) 