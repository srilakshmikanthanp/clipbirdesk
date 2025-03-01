// Google test header files
#include <gtest/gtest.h>

// Local header files
#include "network/packets/authentication.hpp"
#include "network/packets/invalidrequest.hpp"
#include "network/packets/pingpacket.hpp"
#include "network/packets/syncingpacket.hpp"

/**
 * @brief Testing the clipbirdesk Application
 */
auto main(int argc, char **argv) -> int {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
