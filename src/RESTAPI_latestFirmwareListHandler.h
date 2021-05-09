//
// Created by stephane bourque on 2021-05-09.
//

#ifndef UCENTRALFWS_RESTAPI_LATESTFIRMWARELISTHANDLER_H
#define UCENTRALFWS_RESTAPI_LATESTFIRMWARELISTHANDLER_H

#include "RESTAPI_handler.h"

class RESTAPI_latestFirmwareListHandler : public RESTAPIHandler
{
public:
    RESTAPI_latestFirmwareListHandler(const RESTAPIHandler::BindingMap & bindings,Poco::Logger & L)
            : RESTAPIHandler(bindings,L,
                             std::vector<std::string>
                             {  Poco::Net::HTTPRequest::HTTP_GET,
                                Poco::Net::HTTPRequest::HTTP_POST,
                                Poco::Net::HTTPRequest::HTTP_PUT,
                                Poco::Net::HTTPRequest::HTTP_DELETE,
                                Poco::Net::HTTPRequest::HTTP_OPTIONS}) {}
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};


#endif //UCENTRALFWS_RESTAPI_LATESTFIRMWARELISTHANDLER_H
