cmake_minimum_required(VERSION 3.13)
project(owfms VERSION 2.8.0)

set(CMAKE_CXX_STANDARD 17)

if(UNIX AND APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
    set(MYSQL_ROOT_DIR /usr/local/opt/mysql-client)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(UNIX AND NOT APPLE)
    set(PostgreSQL_TYPE_INCLUDE_DIR /usr/include/postgresql)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/build BUILD_NUM)
    if(BUILD_INCREMENT)
        MATH(EXPR BUILD_NUM "${BUILD_NUM}+1")
        file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/build ${BUILD_NUM})
    endif()
else()
    set(BUILD_NUM 1)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/build ${BUILD_NUM})
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_RESULT
            OUTPUT_VARIABLE GIT_HASH)
    if(NOT GIT_RESULT EQUAL "0")
        message(FATAL_ERROR "git rev-parse --short HEAD failed with ${GIT_RESULT}")
    endif()
    string(REGEX REPLACE "\n$" "" GIT_HASH "${GIT_HASH}")
endif()

# set(BUILD_SHARED_LIBS 1)
# add_definitions(-DAWS_CUSTOM_MEMORY_MANAGEMENT)

find_package(OpenSSL    REQUIRED)
find_package(fmt        REQUIRED)
find_package(ZLIB       REQUIRED)
find_package(Poco       REQUIRED COMPONENTS Crypto JWT Net Util NetSSL Data DataSQLite)
find_package(AWSSDK     REQUIRED COMPONENTS s3)
find_package(ZLIB       REQUIRED)

if(SMALL_BUILD)
    find_package(Poco REQUIRED COMPONENTS Crypto JWT Net Util NetSSL Data DataSQLite)
else()
    find_package(CppKafka REQUIRED)
    find_package(PostgreSQL REQUIRED)
    find_package(MySQL REQUIRED)
    find_package(Poco REQUIRED COMPONENTS JSON Crypto JWT Net Util NetSSL Data DataSQLite DataPostgreSQL DataMySQL)
endif()

include_directories(/usr/local/include  /usr/local/opt/openssl/include src include/kafka /usr/local/opt/mysql-client/include)

configure_file(src/ow_version.h.in ${PROJECT_SOURCE_DIR}/src/ow_version.h @ONLY)

add_compile_options(-Wall -Wextra)
add_definitions(-DPOCO_LOG_DEBUG="1")

if(ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable( owfms
        build
        src/ow_version.h.in
        src/framework/CountryCodes.h
        src/framework/KafkaTopics.h
        src/framework/MicroService.h
        src/framework/OpenWifiTypes.h
        src/framework/orm.h
        src/framework/StorageClass.h
        src/framework/MicroServiceErrorHandler.h
        src/framework/UI_WebSocketClientServer.cpp
        src/framework/UI_WebSocketClientServer.h
        src/framework/UI_WebSocketClientNotifications.cpp
        src/framework/UI_WebSocketClientNotifications.h
        src/framework/utils.h
        src/framework/utils.cpp
        src/framework/AppServiceRegistry.h
        src/framework/SubSystemServer.cpp
        src/framework/SubSystemServer.h
        src/framework/RESTAPI_utils.h
        src/framework/AuthClient.cpp
        src/framework/AuthClient.h
        src/framework/MicroServiceNames.h
        src/framework/MicroServiceFuncs.h
        src/framework/OpenAPIRequests.cpp
        src/framework/OpenAPIRequests.h
        src/framework/MicroServiceFuncs.cpp
        src/framework/ALBserver.cpp
        src/framework/ALBserver.h
        src/framework/KafkaManager.cpp
        src/framework/KafkaManager.h
        src/framework/RESTAPI_RateLimiter.h
        src/framework/WebSocketLogger.h
        src/framework/RESTAPI_GenericServerAccounting.h
        src/framework/RESTAPI_SystemConfiguration.h
        src/framework/CIDR.h
        src/framework/RESTAPI_Handler.cpp
        src/framework/RESTAPI_Handler.h
        src/framework/RESTAPI_ExtServer.h
        src/framework/RESTAPI_ExtServer.cpp
        src/framework/RESTAPI_IntServer.cpp
        src/framework/RESTAPI_IntServer.h
        src/framework/RESTAPI_SystemCommand.h
        src/framework/RESTAPI_WebSocketServer.h
        src/framework/EventBusManager.cpp
        src/framework/EventBusManager.h
        src/framework/RESTAPI_PartHandler.h
        src/framework/MicroService.cpp
        src/framework/MicroServiceExtra.h
        src/RESTObjects/RESTAPI_SecurityObjects.h src/RESTObjects/RESTAPI_SecurityObjects.cpp
        src/RESTObjects/RESTAPI_GWobjects.h src/RESTObjects/RESTAPI_GWobjects.cpp
        src/RESTObjects/RESTAPI_FMSObjects.h src/RESTObjects/RESTAPI_FMSObjects.cpp
        src/RESTObjects/RESTAPI_CertObjects.cpp src/RESTObjects/RESTAPI_CertObjects.h
        src/RESTObjects/RESTAPI_OWLSobjects.cpp src/RESTObjects/RESTAPI_OWLSobjects.h
        src/RESTObjects/RESTAPI_ProvObjects.cpp src/RESTObjects/RESTAPI_ProvObjects.h
        src/RESTObjects/RESTAPI_AnalyticsObjects.cpp src/RESTObjects/RESTAPI_AnalyticsObjects.h
        src/RESTObjects/RESTAPI_SubObjects.cpp src/RESTObjects/RESTAPI_SubObjects.h
        src/RESTAPI/RESTAPI_firmwaresHandler.cpp src/RESTAPI/RESTAPI_firmwaresHandler.h
        src/RESTAPI/RESTAPI_firmwareHandler.cpp src/RESTAPI/RESTAPI_firmwareHandler.h
        src/RESTAPI/RESTAPI_historyHandler.cpp src/RESTAPI/RESTAPI_historyHandler.h
        src/RESTAPI/RESTAPI_firmwareAgeHandler.cpp src/RESTAPI/RESTAPI_firmwareAgeHandler.h
        src/RESTAPI/RESTAPI_connectedDevicesHandler.cpp src/RESTAPI/RESTAPI_connectedDevicesHandler.h
        src/RESTAPI/RESTAPI_deviceReportHandler.cpp src/RESTAPI/RESTAPI_deviceReportHandler.h
        src/RESTAPI/RESTAPI_connectedDeviceHandler.cpp src/RESTAPI/RESTAPI_connectedDeviceHandler.h
        src/RESTAPI/RESTAPI_Routers.cpp
        src/Dashboard.cpp src/Dashboard.h
        src/Daemon.cpp src/Daemon.h
        src/StorageService.cpp src/StorageService.h
        src/ManifestCreator.cpp src/ManifestCreator.h
        src/NewConnectionHandler.cpp src/NewConnectionHandler.h
        src/LatestFirmwareCache.cpp src/LatestFirmwareCache.h
        src/DeviceCache.cpp src/DeviceCache.h
        src/FirmwareCache.cpp src/FirmwareCache.h
        src/SDK/Prov_SDK.cpp src/SDK/Prov_SDK.h
        src/AutoUpdater.cpp src/AutoUpdater.h src/SDK/GW_SDK.cpp src/SDK/GW_SDK.h
        src/NewCommandHandler.cpp src/NewCommandHandler.h
        src/storage/orm_history.cpp src/storage/orm_history.h
        src/storage/orm_firmwares.cpp src/storage/orm_firmwares.h
        src/storage/orm_deviceInfo.cpp src/storage/orm_deviceInfo.h
        src/RESTAPI/RESTAPI_deviceInformation_handler.cpp
        src/RESTAPI/RESTAPI_deviceInformation_handler.h)

target_link_libraries( owfms PUBLIC
        ${Poco_LIBRARIES}
        ${MySQL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${AWSSDK_LINK_LIBRARIES}
        fmt::fmt
        CppKafka::cppkafka
    )

