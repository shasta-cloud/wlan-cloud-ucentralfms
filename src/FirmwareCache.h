//
// Created by stephane bourque on 2021-07-26.
//

#ifndef UCENTRALFMS_FIRMWARECACHE_H
#define UCENTRALFMS_FIRMWARECACHE_H

#include <map>
#include <memory>

#include "RESTAPI_FMSObjects.h"
#include "SubSystemServer.h"

namespace uCentral {

    typedef std::map<std::string,std::shared_ptr<FMSObjects::Firmware>> FirmwareCacheMap;

    class FirmwareCache: public SubSystemServer {
    public:
        static FirmwareCache *instance() {
            if (instance_ == nullptr) {
                instance_ = new FirmwareCache;
            }
            return instance_;
        }

        int Start() override;
        void Stop() override;

        std::shared_ptr<FMSObjects::Firmware> GetFirmware(const std::string & DeviceType, const std::string & Revision);
        std::shared_ptr<FMSObjects::Firmware> AddFirmware(const FMSObjects::Firmware &F);


    private:
        static FirmwareCache 	*instance_;
        std::atomic_bool        Running_=false;
        FirmwareCacheMap        Cache_;
        explicit FirmwareCache() noexcept:
                SubSystemServer("FirmwareCache", "FIRMWARE-CACHE", "firmwarecache")
        {
        }
    };

    inline FirmwareCache * FirmwareCache() { return FirmwareCache::instance(); }

}


#endif //UCENTRALFMS_FIRMWARECACHE_H
