#include "acutest.h"

// clang-format off

#define FOREACH_TEST(X) \
    X(EGL_KHR_debug)

#define DECLARE_TEST(NAME) void test_##NAME();
#define ENTRY_TEST(NAME) { #NAME, &test_##NAME },

FOREACH_TEST(DECLARE_TEST)

TEST_LIST = {
    FOREACH_TEST(ENTRY_TEST)
    {NULL, NULL}
};