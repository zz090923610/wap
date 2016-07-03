#
# this module look for gsl (http://www.gnu.org/software/gsl) support
# it will define the following values
#
# GSL_INCLUDE_DIR = where gsl/gsl_version.h can be found
# GSL_LIBRARY     = the library to link against libgsl
# GSL_FOUND       = set to 1 if gsl is found
#

IF(EXISTS ${PROJECT_CMAKE}/GslConfig.cmake)
  INCLUDE(${PROJECT_CMAKE}/GslConfig.cmake)
ENDIF(EXISTS ${PROJECT_CMAKE}/GslConfig.cmake)

IF(Gsl_INCLUDE_DIRS)

  FIND_PATH(GSL_INCLUDE_DIR gsl/gsl_version.h ${Gsl_INCLUDE_DIRS})
  FIND_LIBRARY(GSL_LIBRARY gsl ${Gsl_LIBRARY_DIRS})

ELSE(Gsl_INCLUDE_DIRS)

  SET(TRIAL_LIBRARY_PATHS
    $ENV{GSL_HOME}/lib
    /usr/lib 
    /usr/local/lib
    /opt/lib
    /sw/lib
   )

  SET(TRIAL_INCLUDE_PATHS
    $ENV{GSL_HOME}/include
    /usr/include
    /opt/include
    /usr/local/include
    /sw/include
  )

  FIND_LIBRARY(GSL_LIBRARY gsl ${TRIAL_LIBRARY_PATHS})
  FIND_PATH(GSL_INCLUDE_DIR gsl/gsl_version.h ${TRIAL_INCLUDE_PATHS} )

ENDIF(Gsl_INCLUDE_DIRS)

IF(GSL_INCLUDE_DIR AND GSL_LIBRARY)
  SET(GSL_FOUND 1 CACHE BOOL "Found gsl library")
  IF(QMC_BUILD_STATIC)
    SET(GSL_LIBRARY ${GSL_LIBRARY})
  ELSE(QMC_BUILD_STATIC)
    SET(GSL_LIBRARY ${GSL_LIBRARY} -lgslcblas)
  ENDIF(QMC_BUILD_STATIC)
  MESSAGE(STATUS "GSL_INCLUDE_DIR=${GSL_INCLUDE_DIR}")
  MESSAGE(STATUS "GSL_LIBRARY=${GSL_LIBRARY}")
ELSE(GSL_INCLUDE_DIR AND GSL_LIBRARY)
  MESSAGE(STATUS "Did not find GSL Library")
  SET(GSL_FOUND 0 CACHE BOOL "Not found gsl library")
ENDIF(GSL_INCLUDE_DIR AND GSL_LIBRARY)

MARK_AS_ADVANCED(
  GSL_INCLUDE_DIR 
  GSL_LIBRARY 
  GSL_FOUND
  )
