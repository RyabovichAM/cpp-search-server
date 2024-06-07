#pragma once

#include <deque>
#include <string>
#include <vector>

#include "document.h"
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server) : search_server_(search_server) {
    }

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        int time = 0;
        bool is_empty_request = false; 
    };

    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
    int time_ = 0;
    int empty_request_count_ = 0;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    ++time_;
    auto requests_iter_to_timer = std::find_if(requests_.begin(), requests_.end(), [this] (const QueryResult& q_res) {return (time_ - q_res.time) < min_in_day_; });
    for(auto iter = requests_.begin(); iter != requests_iter_to_timer; ++iter) {
        if ((*iter).is_empty_request) {
            --empty_request_count_;
        }
        requests_.erase(iter);
    }

    QueryResult q_res;
    
    std::vector<Document>  result_documents = search_server_.FindTopDocuments(raw_query, document_predicate);
    if(result_documents.empty()) {
        ++empty_request_count_;
        q_res.is_empty_request = true;
    }

    q_res.time = time_;
    requests_.push_back(q_res);

    return result_documents;
}