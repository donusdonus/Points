#include "Point.h"
#include <iostream>
#include <cmath>

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cout << "[FAIL] " << msg << std::endl; \
            return false; \
        } \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cout << "[FAIL] " << msg << " | Expected: " << (b) << " Got: " << (a) << std::endl; \
            return false; \
        } \
    } while(0)

#define RUN_TEST(fn) \
    do { \
        if (fn()) std::cout << "[PASS] " << #fn << std::endl; \
        else failures++; \
    } while(0)

bool test_int_point() {
    Point p(INT32_T, "INT32", 1);
    int32_t v = 1234;
    p.SetValue(v);

    uint32_t test = p.GetValue<int32_t>();

    ASSERT_EQ(p.GetValue<int32_t>(), v, "INT32 value mismatch");
    return true;
}

bool test_array_point() {
    uint8_t arr[4] = {1,2,3,4};
    Point p(UINT8_T, "Array", arr, 4);
    ASSERT_EQ(p.Count(), 4, "Array count mismatch");
    ASSERT_EQ(p.GetValue<uint8_t>(2), 3, "Array value mismatch");
    return true;
}

bool test_group_insert_find() {
    Point group(GROUP_T,"X","G");
    Point a(INT8_T, "A", 88);
    Point b(FLOAT_T, "B", 3.14f);

    ASSERT_TRUE(group.AddSlot(a), "Add A fail");
    ASSERT_TRUE(group.AddSlot(b), "Add B fail");

    Point *found = group.FindSlot("B");
    ASSERT_TRUE(found != nullptr, "Find B failed");
    ASSERT_EQ(found->GetValue<float>(), 3.14f, "Value B mismatch");
    return true;
}

int main() {
    int failures = 0;

    RUN_TEST(test_int_point);
    RUN_TEST(test_array_point);
    RUN_TEST(test_group_insert_find);

    std::cout << "Total Failures: " << failures << std::endl;
    return failures == 0 ? 0 : 1;
}
