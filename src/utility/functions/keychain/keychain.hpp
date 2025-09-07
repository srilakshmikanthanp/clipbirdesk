#include <QFuture>
#include <QPromise>
#include <QScopeGuard>
#include <functional>

#include "qt6keychain/keychain.h"

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
template <typename JobT, typename Parser>
auto toFuture(JobT *job, Parser&& parser) -> QFuture<std::optional<decltype(parser(job))>> {
  auto promise = std::make_shared<QPromise<std::optional<decltype(parser(job))>>>();
  QFuture<std::optional<decltype(parser(job))>> future = promise->future();

  auto handleJobFinished = [promise, job, parser](QKeychain::Job * kjob) mutable {
    auto cleanup = qScopeGuard([=]() { job->deleteLater(); });
    JobT *j = static_cast<JobT *>(kjob);

    if (j->error() == QKeychain::Error::EntryNotFound) {
      promise->addResult(std::nullopt);
      promise->finish();
      return;
    } else if (j->error()) {
      promise->setException(std::make_exception_ptr(std::runtime_error(j->errorString().toStdString())));
      return;
    }

    try {
      promise->addResult(parser(j));
      promise->finish();
    } catch (const std::exception &e) {
      promise->setException(std::current_exception());
    }
  };

  QObject::connect(job, &QKeychain::Job::finished, handleJobFinished);
  job->start();
  return future;
}

template <typename JobT, typename MemberFn>
auto toFuture(JobT *job, MemberFn memberFn) -> QFuture<std::optional<decltype((job->*memberFn)())>> {
  return toFuture(job, [memberFn](JobT *j) { return (j->*memberFn)(); });
}

template <typename JobT>
auto toFuture(JobT *job) -> QFuture<void> {
  auto promise = std::make_shared<QPromise<void>>();
  QFuture<void> future = promise->future();

  auto handleJobFinished = [promise, job](QKeychain::Job * kjob) mutable {
    auto cleanup = qScopeGuard([=]() { job->deleteLater(); });
    JobT *j = static_cast<JobT *>(kjob);

    if (j->error()) {
      promise->setException(std::make_exception_ptr(std::runtime_error(j->errorString().toStdString())));
      return;
    }

    promise->finish();
  };

  QObject::connect(job, &QKeychain::Job::finished, handleJobFinished);
  job->start();
  return future;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
