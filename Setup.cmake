IF(MINGW)
    INCLUDE(${OE_CURRENT_EXTENSION_DIR}/FindWinsock2.cmake)

    IF (NOT WINSOCK2_FOUND) 
        MESSAGE ("WARNING: Could not find winsock2 libraries  - depending targets will be disabled.")
    ENDIF (NOT WINSOCK2_FOUND)
ENDIF(MINGW)

