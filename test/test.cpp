// Google test header files
#include <gtest/gtest.h>

// Local header files
#include "packets/authentication.hpp"
#include "packets/certificate_exchange_packet.hpp"
#include "packets/invalidrequest.hpp"
#include "packets/pingpongpacket.hpp"
#include "packets/syncingpacket.hpp"

/**
 * @brief Testing the clipbirdesk Application
 */
auto main(int argc, char **argv) -> int {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
