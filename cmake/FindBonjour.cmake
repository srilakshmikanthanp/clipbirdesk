# This cmake File sued to Find the bonjour library
# If the bonjour library is found, BONJOUR_FOUND is set to true
# and the following variables are set:
#   BONJOUR_INCLUDE_DIR   - the bonjour include directory
#   BONJOUR_LIBRARIES     - the libraries needed to use bonjour
# You need to pass the following variables to cmake:
#   BONJOUR_DIR           - the root directory of bonjour

# try to find dns_sd.h with path hind as BONJOUR_DIR/include
find_path(BONJOUR_INCLUDE_DIR dns_sd.h
  PATHS ${BONJOUR_DIR}/include
)

# log the result of the search
if(BONJOUR_INCLUDE_DIR)
  message(STATUS "Found dns_sd.h: ${BONJOUR_INCLUDE_DIR}")
else(BONJOUR_INCLUDE_DIR)
  message(STATUS "dns_sd.h not found")
endif(BONJOUR_INCLUDE_DIR)

# try to find dnssd.lib with path hind as BONJOUR_DIR/lib
find_library(BONJOUR_LIBRARIES dnssd
  PATHS ${BONJOUR_DIR}/lib/X64
)

# log the result of the search
if(BONJOUR_LIBRARIES)
  message(STATUS "Found dnssd: ${BONJOUR_LIBRARIES}")
else(BONJOUR_LIBRARIES)
  message(STATUS "dnssd not found")
endif(BONJOUR_LIBRARIES)

# include the FindPackageHandleStandardArgs module
include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set BONJOUR_FOUND to TRUE
find_package_handle_standard_args(Bonjour DEFAULT_MSG
  BONJOUR_INCLUDE_DIR BONJOUR_LIBRARIES
)
