# - Try to find winsock2
# Once done this will define
#
#  WINSOCK2_FOUND - system has winsock2
#  WINSOCK2_LIBRARY - Link these to winsock2
#

FIND_LIBRARY(WINSOCK2_LIBRARY
  NAMES 
  libws2_32.a
  PATHS
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_SOURCE_DIR}/libraries
  ${PROJECT_SOURCE_DIR}/libraries/winsock2/lib
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)

IF(WINSOCK2_LIBRARY)
  SET(WINSOCK2_FOUND TRUE)
ENDIF(WINSOCK2_LIBRARY)

# show the WINSOCK2_LIBRARY variables only in the advanced view
IF(WINSOCK2_FOUND)
  MARK_AS_ADVANCED(WINSOCK2_LIBRARY)
ENDIF(WINSOCK2_FOUND)
