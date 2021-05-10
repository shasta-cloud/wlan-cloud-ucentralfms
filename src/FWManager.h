//
// Created by stephane bourque on 2021-05-10.
//

#ifndef UCENTRALFWS_FWMANAGER_H
#define UCENTRALFWS_FWMANAGER_H

#include <queue>

#include "SubSystemServer.h"
#include <aws/s3/S3Client.h>
#include "uAuthService.h"

namespace uCentral::FWManager {

    int Start();
    void Stop();
    bool AddJob(const std::string &UUID, const uCentral::Auth::APIKeyEntry & Entry);

    struct JobId {
        std::string UUID;
        uCentral::Auth::APIKeyEntry Entry;
    };

class Service : public SubSystemServer, Poco::Runnable {
    public:

        Service() noexcept;

        friend int Start();
        friend void Stop();

        static Service *instance() {
            if (instance_ == nullptr) {
                instance_ = new Service;
            }
            return instance_;
        }
        friend bool AddJob(const std::string &UUID, const uCentral::Auth::APIKeyEntry & Entry);
        void run() override;

    private:
        static Service          *instance_;
        std::queue<JobId>       Jobs_;
        Poco::Thread            Worker_;
        std::atomic_bool        Running_=false;
        std::string             S3BucketName_;
        std::string             S3Region_;
        std::string             S3Key_;
        std::string             S3Secret_;
        uint64_t                S3Retry_;
        int Start() override;
        void Stop() override;
        bool AddJob(const std::string &UUID, const uCentral::Auth::APIKeyEntry & Entry);

        bool SendToS3(const std::string & JSONObjectName , const std::string & JSONDocFileName,
             const std::string & ImageObjectName, const std::string & ImageFileName);
        bool SendObjectToS3(std::shared_ptr<Aws::S3::S3Client> & Client, const std::string &ObjectName, const std::string & ObjectFileName);
    };

}   // namespace

#endif //UCENTRALFWS_FWMANAGER_H
