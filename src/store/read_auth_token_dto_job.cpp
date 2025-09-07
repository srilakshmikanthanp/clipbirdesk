#include "read_auth_token_dto_job.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
syncing::wan::AuthTokenDto ReadAuthTokenDtoJob::authToken() const {
  nlohmann::json json = nlohmann::json::parse(textData().toStdString());
  return json.get<syncing::wan::AuthTokenDto>();
}
}
