cmake_minimum_required(VERSION 2.8)

project(swr)

SET(SWR_LIB
    model/model.cpp
    parser/parser.cpp
    parser/objparser.cpp
    render/renderer.cpp
    render/iodevice/tgaimage.cpp
    render/iodevice/tgadevice.cpp
    utils/color.cpp
    )

include_directories(model parser render render/iodevice utils)

add_library(swr STATIC ${SWR_LIB})