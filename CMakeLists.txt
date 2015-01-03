# ----------------------------------------------------------------------------
# Introduction
#
# The project uses cmake to build its components. This is the main cmake file
# that should be used like so:
#     mkdir build && cd build && cmake $OPTIONS ../src && make && make install
# $OPTIONS are a set of definitions of the form `-DDEFINITION=Value`
# Here are valid definitions:
#
#   -DCMAKE_BUILD_TYPE=Debug
#   -DCMAKE_BUILD_TYPE=Release
#       Either enable or disable development aids;
#
#   -DCMAKE_INSTALL_PREFIX:PATH=/usr
#       The base path for installed components;
#
# To get the full list of make targets run `make help` in build directory. Here
# are some important ones:
#
#   install
#       copies installable components to their intended places
#
#   doc
#       creates documentation using Doxygen
#
#   internal-test
#       creates internal test executable based on Google Test suite
#
# ============================================================================

# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 3.0.0)

include(pile_support)

# ============================================================================

# ----------------------------------------------------------------------------
# customisation

pileProject("CLB" "0;0;1;1")
project("CLB" VERSION "${CLB_VERSION}" LANGUAGES C CXX)
pileProjectCommon()

# ============================================================================

# ----------------------------------------------------------------------------
# Specifics



# ============================================================================

# ----------------------------------------------------------------------------
# Subdirectories

add_subdirectory("lib_clb")
add_subdirectory("app_clb")
if( CLB_BUILD_TESTS)
	add_subdirectory("tests")
endif( CLB_BUILD_TESTS)

# ============================================================================

pileProjectEnd()
