//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H

#include "indexer_inf.h"
#include "event_batch.h"

#include <string>
#include <memory>
#include <functional>

namespace splunkhec {

class HecChannel {
public:
    // HecChannel doesn't own indexer
    explicit HecChannel(IndexerInf* indexer);

    const IndexerInf* indexer() const {
        return indexer_;
    }

    IndexerInf* indexer() {
        return indexer_;
    }

    void send(const std::shared_ptr<EventBatch>& batch) {
        indexer_->send(batch);
    }

    const std::string& id() const {
        return uuid_;
    }

    bool operator ==(const HecChannel& rhs) const {
        return uuid_ == rhs.uuid_;
    }

private:
    IndexerInf* indexer_;
    std::string uuid_;
};

} // namespace splunkhec


#endif //SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H
