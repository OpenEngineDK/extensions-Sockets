
# Create the extension library
ADD_LIBRARY(Extensions_Sockets
  Network/TCPSocket.h
  Network/TCPSocket.cpp
)
IF(MINGW)
    TARGET_LINK_LIBRARIES(Extensions_Sockets
        ${WINSOCK2_LIBRARY}
    )
ENDIF(MINGW)
