#include <iostream>
#include "gtest/gtest.h"
#include <math.h>
#include <cstdio>
#include <thread>
#include <cstdlib>
#include "../wkhttp/HttpStatus.h"

using namespace std;

TEST(HttpServerTest, TransactionRollBack) {
    wk::HttpStatus::Code status = wk::HttpStatus::Code::Ok;
    EXPECT_EQ("OK", wk::HttpStatus::reasonPhrase(status));
    EXPECT_TRUE(true);
}