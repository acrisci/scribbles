cmake_minimum_required(VERSION 2.8.12.1)

set(DUMMY_MAJOR 0)
set(DUMMY_MINOR 0)
set(DUMMY_PATCH 1)
set(DUMMY_VERSION ${DUMMY_MAJOR}.${DUMMY_MINOR}.${DUMMY_PATCH})

set(CPACK_PACKAGE_VERSION ${DUMMY_VERSION})
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "a dummy package for testing")
set(CPACK_PACKAGE_NAME "dummy")
set(CPACK_PACKAGE_CONTACT nobody@example.com)

install(FILES has-dummy.txt DESTINATION /etc/has-dummy.txt)

include(CPack)
