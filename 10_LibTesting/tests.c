#include <check.h>
#include "buf.h"

START_TEST(test_buf_basic)
{
    float *a = 0;
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    
    ck_assert_int_eq(buf_size(a), 3);
    ck_assert_msg(a[2] == (float)1.3, "Value mismatch");
    
    buf_free(a);
}
END_TEST

START_TEST(test_buf_capacity)
{
    // 测试容量增长逻辑
    int *ai = 0;
    buf_grow(ai, 100);
    ck_assert_int_ge(buf_capacity(ai), 100);
    buf_free(ai);
}
END_TEST

Suite *buf_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Buf");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_buf_basic);
    tcase_add_test(tc_core, test_buf_capacity);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = buf_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
