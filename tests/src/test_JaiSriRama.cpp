
#include "test_JaiSriRama.h"

TEST_F(JaiSriRamTestFixture, NoThrowOnCall) {
    EXPECT_NO_THROW(printJaisriRam());
}


TEST(JaiSriRamTest, getGodName) {
    // This is a simple stub test — replace with actual logic or mock testing.
    EXPECT_EQ("Ganesha",getGodName());
}