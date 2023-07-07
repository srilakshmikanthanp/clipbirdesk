#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <gtest/gtest.h>

#include "../../../utility/functions/coding.hpp"
#include "../../../types/types.hpp"
/**
 * @brief testing netToHost function with different types
 * int64_t, int32_t, int16_t, int8_t
 */
TEST(HostToNetTest, TestingHostToNet) {
  // using the hostToNet function
  using srilakshmikanthanp::clipbirdesk::utility::functions::coding::hostToNet;

  // int64_t
  int64_t i64_t = 0x123456789ABCDEF0;
  int64_t o64_t = 0xF0DEBC9A78563412;

  EXPECT_EQ(hostToNet(i64_t), o64_t);

  // int16_t
  int16_t i16_t = 0x1234;
  int16_t o16_t = 0x3412;

  EXPECT_EQ(hostToNet(i16_t), o16_t);

  // int8_t
  int8_t i8_t = 0x12;
  int8_t o8_t = 0x12;

  EXPECT_EQ(hostToNet(i8_t), o8_t);

  // int32_t
  int32_t i32_t = 0x12345678;
  int32_t o32_t = 0x78563412;

  EXPECT_EQ(hostToNet(i32_t), o32_t);
}

/**
 * @brief testing netToHost function with different types
 * int64_t, int32_t, int16_t, int8_t
 */
TEST(NetToHostTest, TestingNetToHost) {
  // using the netToHost function
  using srilakshmikanthanp::clipbirdesk::utility::functions::coding::netToHost;

  // int64_t
  int64_t i64_t = 0x123456789ABCDEF0;
  int64_t o64_t = 0xF0DEBC9A78563412;

  EXPECT_EQ(netToHost(i64_t), o64_t);

  // int16_t
  int16_t i16_t = 0x1234;
  int16_t o16_t = 0x3412;

  EXPECT_EQ(netToHost(i16_t), o16_t);

  // int8_t
  int8_t i8_t = 0x12;
  int8_t o8_t = 0x12;

  EXPECT_EQ(netToHost(i8_t), o8_t);

  // int32_t
  int32_t i32_t = 0x12345678;
  int32_t o32_t = 0x78563412;

  EXPECT_EQ(netToHost(i32_t), o32_t);
}

/**
 * @brief testing toBytes function with different types
 * int64_t, int32_t, int16_t, int8_t
 */
TEST(ToBytesTest, TestingToBytes) {
  // using the toBytes function
  using srilakshmikanthanp::clipbirdesk::utility::functions::coding::toBytes;

  // using varies_t type
  using varies_t = srilakshmikanthanp::clipbirdesk::types::varies_t;

  // int64_t
  int64_t i64_t = 0x123456789ABCDEF0;
  varies_t o64_t = {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};

  EXPECT_EQ(toBytes(i64_t), o64_t);

  // int16_t
  int16_t i16_t = 0x1234;
  varies_t o16_t = {0x34, 0x12};

  EXPECT_EQ(toBytes(i16_t), o16_t);

  // int8_t
  int8_t i8_t = 0x12;
  varies_t o8_t = {0x12};

  EXPECT_EQ(toBytes(i8_t), o8_t);

  // int32_t
  int32_t i32_t = 0x12345678;
  varies_t o32_t = {0x78, 0x56, 0x34, 0x12};

  EXPECT_EQ(toBytes(i32_t), o32_t);
}

/**
 * @brief testing fromBytes function with different types
 * int64_t, int32_t, int16_t, int8_t
 */
TEST(FromBytesTest, TestingFromBytes) {
  // using the fromBytes function
  using srilakshmikanthanp::clipbirdesk::utility::functions::coding::fromBytes;

  // using varies_t type
  using varies_t = srilakshmikanthanp::clipbirdesk::types::varies_t;

  // int64_t
  varies_t i64_t = {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};
  int64_t o64_t = 0x123456789ABCDEF0;

  EXPECT_EQ(fromBytes<int64_t>(i64_t), o64_t);

  // int16_t
  varies_t i16_t = {0x34, 0x12};
  int16_t o16_t = 0x1234;

  EXPECT_EQ(fromBytes<int16_t>(i16_t), o16_t);

  // int8_t
  varies_t i8_t = {0x12};
  int8_t o8_t = 0x12;

  EXPECT_EQ(fromBytes<int8_t>(i8_t), o8_t);

  // int32_t
  varies_t i32_t = {0x78, 0x56, 0x34, 0x12};
  int32_t o32_t = 0x12345678;

  EXPECT_EQ(fromBytes<int32_t>(i32_t), o32_t);
}
