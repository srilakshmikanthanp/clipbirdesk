#pragma once

#include <QDateTime>
#include <QString>
#include <nlohmann/json.hpp>

namespace nlohmann {
template <>
struct adl_serializer<QString> {
  static void from_json(const json& j, QString& s) {
    s = QString::fromStdString(j.get<std::string>());
  }

  static void to_json(json& j, const QString& s) {
    j = s.toStdString();
  }
};

template <>
struct adl_serializer<QDateTime> {
  static void from_json(const json& j, QDateTime& dt) {
    dt = QDateTime::fromString(QString::fromStdString(j.get<std::string>()), Qt::ISODate);
  }

  static void to_json(json& j, const QDateTime& dt) {
    j = dt.toString(Qt::ISODate).toStdString();
  }
};

template <>
struct adl_serializer<QByteArray> {
  static void from_json(const json& j, QByteArray& b) {
    b = QByteArray::fromBase64(j.get<QString>().toUtf8());
  }

  static void to_json(json& j, const QByteArray& b) {
    j = b.toBase64();
  }
};

template <>
struct adl_serializer<QPair<QString, QByteArray>> {
  static void from_json(const json& j, QPair<QString, QByteArray>& p) {
    p.first = j.at("first").get<QString>();
    p.second = j.at("second").get<QByteArray>();
  }

  static void to_json(json& j, const QPair<QString, QByteArray>& p) {
    j = json{{"first", p.first}, {"second", p.second.toBase64()}};
  }
};

template <>
struct adl_serializer<QVector<QPair<QString, QByteArray>>> {
  static void from_json(const json& j, QVector<QPair<QString, QByteArray>>& vec) {
    vec.clear();
    for (const auto& item : j) {
      vec.append(item.get<QPair<QString, QByteArray>>());
    }
  }

  static void to_json(json& j, const QVector<QPair<QString, QByteArray>>& vec) {
    j = json::array();
    for (const auto& item : vec) {
      j.push_back(item);
    }
  }
};
}  // namespace nlohmann
