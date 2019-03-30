CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}
)

project( Ks )

set( CLAN_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/io/StandardIO.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/string/encoding.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/string/path.cpp
    )

add_library( Ks STATIC ${CLAN_SOURCE} )