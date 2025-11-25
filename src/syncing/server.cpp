#include "server.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
Server::Server(const common::types::SslConfig sslConfig, QObject *parent): QObject(parent), sslConfig(sslConfig) {}
Server::~Server() {}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
