//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_H

#include "indexer_inf.h"
#include "hec_channel.h"
#include "config.h"

#include <cpprest/http_client.h>
#include <string>

namespace splunkhec {

class PollerInf;

class Indexer final: public IndexerInf {
public:
    Indexer(const std::string& base_uri, const std::string& token, const std::shared_ptr<PollerInf>& poller, const Config& config);
    void send(const std::shared_ptr<EventBatch>& batch) const override;
    bool has_backpressure() const override {
        return false;
    }

private:
    std::string base_uri_;
    std::string token_;
    std::shared_ptr<PollerInf> poller_;
    Config config_;
    std::unique_ptr<web::http::client::http_client> client_;
    HecChannel channel_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_H