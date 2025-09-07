#include "constants/constants.hpp"
#include "auth_token_dto.hpp"
#include "basic_auth_request_dto.hpp"
#include "auth_repository.hpp"
#include "auth_api_client.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class AuthApiRepository : public AuthRepository {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(AuthApiRepository)

 private:

  AuthApiClient* m_apiClient;

 public:

  /**
   * @brief Construct a new AuthRepository object
   *
   * @param parent Parent object
   */
  explicit AuthApiRepository(AuthApiClient *apiClient, QObject* parent = nullptr);

  /**
   * @brief Authenticate user and get auth token
   */
  QFuture<AuthTokenDto> signIn(BasicAuthRequestDto dto);

  /**
   * @brief Destroy the AuthRepository object
   */
  virtual ~AuthApiRepository();
};
}
