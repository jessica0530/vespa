# Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
vespa_add_library(memfilepersistence_device OBJECT
    SOURCES
    device.cpp
    disk.cpp
    partition.cpp
    directory.cpp
    devicemapper.cpp
    devicemanager.cpp
    ioevent.cpp
    partitionmonitor.cpp
    mountpointlist.cpp
    DEPENDS
)
