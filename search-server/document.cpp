#include "document.h"

Document::Document(int s_id, double s_relevance, int s_rating)
    : id(s_id)
    , relevance(s_relevance)
    , rating(s_rating) {
}

std::ostream& operator<<(std::ostream& os, const Document& document) {
    using namespace std::literals;
os << "{ "s
    << "document_id = "s << document.id << ", "s
    << "relevance = "s << document.relevance << ", "s
    << "rating = "s << document.rating << " }"s;
return os;
}