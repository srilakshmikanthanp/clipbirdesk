#include "secure_storage.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
/**
 * @brief Set the Hub JWT token
 */
QKeychain::WritePasswordJob* SecureStorage::setHubAuthTokenToken(const syncing::wan::AuthTokenDto& token){
  auto job = new QKeychain::WritePasswordJob(constants::getAppName(), this);
  job->setKey(hubJwtTokenKey);
  nlohmann::json j = token;
  job->setTextData(QString::fromStdString(j.dump()));
  return job;
}

/**
 * @brief Get the Hub JWT token
 */
ReadAuthTokenDtoJob* SecureStorage::getHubAuthToken() {
  auto job = new ReadAuthTokenDtoJob(constants::getAppName(), this);
  job->setKey(hubJwtTokenKey);
  return job;
}

/**
 * @brief Remove the Hub JWT token
 */
QKeychain::DeletePasswordJob* SecureStorage::removeHubJwtToken() {
  auto job = new QKeychain::DeletePasswordJob(constants::getAppName(), this);
  job->setKey(hubJwtTokenKey);
  return job;
}

/**
 * @brief set the Hub Host Device
 */
QKeychain::WritePasswordJob* SecureStorage::setHubHostDevice(const syncing::wan::HubHostDevice& device) {
  auto job = new QKeychain::WritePasswordJob(constants::getAppName(), this);
  job->setKey(hubHostDeviceKey);
  nlohmann::json j = device;
  job->setTextData(QString::fromStdString(j.dump()));
  return job;
}

/**
 * @brief Get the Hub Host Device
 */
ReadHubHostDeviceJob* SecureStorage::getHubHostDevice() {
  auto job = new ReadHubHostDeviceJob(constants::getAppName(), this);
  job->setKey(hubHostDeviceKey);
  return job;
}

/**
 * @brief Remove the Hub Host Device
 */
QKeychain::DeletePasswordJob* SecureStorage::removeHubHostDevice() {
  auto job = new QKeychain::DeletePasswordJob(constants::getAppName(), this);
  job->setKey(hubHostDeviceKey);
  return job;
}

SecureStorage::SecureStorage(QObject *parent) : QObject(parent) {}

SecureStorage::~SecureStorage() = default;

SecureStorage &SecureStorage::instance() {
  static SecureStorage instance;
  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::storage
