#include "Post.h"

string Post::getBody() const {
    return body;
}

void Post::setBody(const string& newBody) {
    body = newBody;
}

vector<string> Post::getTopics() const {
    return topics;
}

void Post::setTopics(const vector<string>& newTopics) {
    topics = newTopics;
}
