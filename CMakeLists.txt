cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11 -O3")

find_library(BOOST_REGEX boost_regex)

add_executable(seqextr
    SequenceExtractor.cpp)

target_link_libraries(seqextr
    ${BOOST_REGEX})

add_executable(mutator
    CGMutator.cpp)

target_link_libraries(mutator
    ${BOOST_REGEX})

add_executable(rmsdcalc
    RMSDCalculator.cpp)

target_link_libraries(rmsdcalc
    ${BOOST_REGEX})

add_executable(superimpose
    SuperImposer.cpp)
