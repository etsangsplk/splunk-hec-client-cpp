//
// Created by kchen on 12/3/17.
//

#include "indexer.h"
#include "http_client_factory.h"
#include "poller_inf.h"

using namespace std;
using namespace web::http;

namespace splunkhec {

Indexer::Indexer(const string& base_uri, const string& token, const shared_ptr<PollerInf>& poller, const Config& config)
        : base_uri_(base_uri), token_("Splunk " + token), poller_(poller), config_(config), channel_(*this) {
    HttpClientFactory factory(config_);
    client_ = move(factory.create(base_uri_));
}

void Indexer::send(const shared_ptr<EventBatch>& batch) const {
    http_request req{methods::POST};
    auto& headers = req.headers();
    headers.set_content_type(batch->content_type());
    headers.add("X-Splunk-Request-Channel", channel_.id());
    headers.add("Authorization", token_);
    req.set_body(batch->serialize());
    req.set_request_uri(batch->rest_endpoint());

    pplx::task<void> res = client_->request(req).then([&](pplx::task<http_response> t) {
        try {
            http_response response = t.get();
            string str_resp = response.extract_string().get();
            poller_->add(channel_, batch, str_resp);
        } catch (const http_exception& ex) {
            poller_->fail(channel_, batch, ex);
        }
    });
    res.get();
}

} // namespace splunkhec