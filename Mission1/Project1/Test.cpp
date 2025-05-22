#ifdef DEBUG
#include <gmock/gmock.h>
#include "fix_keyword.cpp"

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

TEST(TC, TC1)
{
	EXPECT_EQ(FixKeyword("water","monday"), "");
}
#endif // DEBUG