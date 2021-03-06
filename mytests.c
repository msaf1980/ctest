#include <stdlib.h>
#include "ctest.h"

// basic test without setup/teardown
CTEST(suite1, test1) {
}

// there are many different ASSERT macro's (see ctest.h)
CTEST(suite1, test2_fail) {
    ASSERT_EQUAL(1,2);
}

CTEST(suite2, test1_fail) {
    ASSERT_STR("foo", "bar");
}

CTEST(suite2, test1_d_fail) {
    ASSERT_STR_D("foo", "bar", "must fail");
}

CTEST(suite3, test3) {
}


// A test suite with a setup/teardown function
// This is converted into a struct that's automatically passed to all tests in the suite
CTEST_DATA(memtest) {
    unsigned char* buffer;
};

// Optional setup function for suite, called before every test in suite
CTEST_SETUP(memtest) {
    CTEST_LOG("%s() data=%p buffer=%p", __func__, (void*)data, (void*)data->buffer);
    data->buffer = (unsigned char*)malloc(1024);
}

// Optional teardown function for suite, called after every test in suite
CTEST_TEARDOWN(memtest) {
    CTEST_LOG("%s() data=%p buffer=%p", __func__, (void*)data, (void*)data->buffer);
    if (data->buffer) free(data->buffer);
}

// These tests are called with the struct* (named data) as argument
CTEST2(memtest, test1) {
    CTEST_LOG("%s()  data=%p  buffer=%p", __func__, (void*)data, (void*)data->buffer);
}

CTEST2_SKIP(memtest, test3) {
    (void)data;
    ASSERT_FAIL();
}

CTEST2(memtest, test2_fail) {
    CTEST_LOG("%s()  data=%p  buffer=%p", __func__, (void*)data, (void*)data->buffer);
    ASSERT_FAIL_D("must fail");
}


CTEST_DATA(fail) {
    int unused;
};

// Asserts can also be used in setup/teardown functions
CTEST_SETUP(fail) {
    (void)data;
    ASSERT_FAIL_D("must fail");
}

CTEST2(fail, test1) {
    (void)data;
}



CTEST_DATA(weaklinkage) {
    int number;
};

// This suite has data, but no setup/teardown
CTEST2(weaklinkage, test1) {
    (void)data;
    CTEST_LOG("%s()", __func__);
}

CTEST2(weaklinkage, test2) {
    (void)data;
    CTEST_LOG("%s()", __func__);
}


CTEST_DATA(nosetup) {
    int value;
};

CTEST_TEARDOWN(nosetup) {
    (void)data;
    CTEST_LOG("%s()", __func__);
}

CTEST2(nosetup, test1) {
    (void)data;
    CTEST_LOG("%s()", __func__);
}


// more ASSERT examples
CTEST(ctest, test_assert_str) {
    ASSERT_STR("foo", "foo");
    ASSERT_STR_D("foo", "bar", "must fail");
}

CTEST(ctest, test_assert_equal) {
    ASSERT_EQUAL(123, 123);
    ASSERT_EQUAL_D(123, 456, "must fail");
}

CTEST(ctest, test_assert_not_equal) {
    ASSERT_NOT_EQUAL(123, 456);
    ASSERT_NOT_EQUAL_D(123, 123, "must fail");
}

CTEST(ctest, test_assert_interval) {
    ASSERT_INTERVAL(10, 20, 15);
    ASSERT_INTERVAL_D(1000, 2000, 3000, "must fail");
}

CTEST(ctest, test_assert_null) {
    ASSERT_NULL(NULL);
    ASSERT_NULL_D((void*)0xdeadbeef, "must fail");
}

CTEST(ctest, test_assert_not_null_const) {
    ASSERT_NOT_NULL((const char*)"hallo");
}

CTEST(ctest, test_assert_not_null) {
    ASSERT_NOT_NULL((void*)0xdeadbeef);
    ASSERT_NOT_NULL_D(NULL, "must fail");
}

CTEST(ctest, test_assert_true) {
    ASSERT_TRUE(1);
    ASSERT_TRUE_D(0, "must fail");
}

CTEST(ctest, test_assert_false) {
    ASSERT_FALSE(0);
    ASSERT_FALSE_D(1, "must fail");
}

CTEST_SKIP(ctest, test_skip) {
    ASSERT_FAIL();
}

CTEST(ctest, test_assert_fail) {
    ASSERT_FAIL_D("must fail");
}

/* Test that NULL-strings won't result in segv */
CTEST(ctest, test_null_null) {
    ASSERT_STR(NULL, NULL);
}

CTEST(ctest, test_null_string_fail) {
    ASSERT_STR_D(NULL, "shouldfail", "must fail");
}

CTEST(ctest, test_string_null_fail) {
    ASSERT_STR_D("shouldfail", NULL, "must fail");
}

CTEST(ctest, test_string_diff_ptrs) {
    const char *str = "abc\0abc";
    ASSERT_STR(str, str+4);
}

CTEST(ctest, test_large_numbers) {
    unsigned long exp = 4200000000u;
    ASSERT_EQUAL_U(exp, 4200000000u);
    ASSERT_NOT_EQUAL_U(exp, 1200000000u);
}

CTEST(ctest, test_ctest_err_fail) {
    CTEST_ERR("error log");
}

CTEST(ctest, test_dbl_near) {
    double a = 0.000111;
    ASSERT_DBL_NEAR(0.0001, a);
}

CTEST(ctest, test_dbl_near_d_fail) {
    double a = 0.000211;
    ASSERT_DBL_NEAR_D(0.0001, a, "must fail"); /* will fail */
}

CTEST(ctest, test_dbl_near_tol_fail) {
    double a = 0.000111;
    ASSERT_DBL_NEAR_TOL_D(0.0001, a, 1e-5, "must fail"); /* will fail */
}

CTEST(ctest, test_dbl_near_tol_d_fail) {
    char buf[1024];
    double a = 0.000111;
    ASSERT_DBL_NEAR_TOL_D(0.0001, a, 1e-5, CTEST_DESCRIPTION_FORMAT(buf, sizeof(buf), "must fail %f", a)); /* will fail */
}

CTEST(ctest, test_dbl_far) {
    double a = 1.1;
    ASSERT_DBL_FAR(1., a);
    ASSERT_DBL_FAR_TOL(1., a, 0.01);
}

CTEST(ctest, description_format) {
    char buf[1024];
    CTEST_DESCRIPTION_FORMAT(buf, sizeof(buf), "test %d %s", 1, "end");
    ASSERT_STR("test 1 end", buf);
}
