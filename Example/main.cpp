#include <iostream>
#include <chrono>
#include <cassert>
#include <cstring>
#include "Point.h"

using namespace std;
using namespace std::chrono;

#define ASSERT(expr, message) \
    if (!(expr)) { \
        cout << "[FAIL] " << message << endl; \
        failed++; \
    } else { \
        passed++; \
    }

int failed = 0, passed = 0;

// === Test 1: Constructor & Destructor ===
void test_ConstructorDestructor() {
    cout << "\n[TEST 1] Constructor & Destructor" << endl;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        Point pt(VAR_UINT32, "cons", 10);
        ASSERT(pt.GetType() == VAR_UINT32, "Constructor failed at iteration " + to_string(i));
    }

    for (int i = 0; i < 100; ++i) {
        Point* pt = new Point(VAR_FLOAT, "heap", 5);
        delete pt;
    }

    auto stop = high_resolution_clock::now();
    cout << "[BENCHMARK] Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
}

// === Test 2: SetValue / GetValue ===
template<typename T>
void test_SetGet_Value(isType type, const char* typeName, T val1, T val2) {
    cout << "\n[TEST 2] SetValue/GetValue<" << typeName << "> with 100 elements" << endl;
    auto start = high_resolution_clock::now();

    Point pt(type, "testPoint", 100);

    for (int i = 0; i < 100; ++i)
        ASSERT(pt.SetValue<T>(val1, i), "SetValue failed at index " + to_string(i));

    for (int i = 0; i < 100; ++i)
        ASSERT(pt.GetValue<T>(i) == val1, "GetValue mismatch at index " + to_string(i));

    for (int i = 50; i < 60; ++i)
        ASSERT(pt.SetValue<T>(val2, i), "Overwrite SetValue failed at index " + to_string(i));

    for (int i = 50; i < 60; ++i)
        ASSERT(pt.GetValue<T>(i) == val2, "Overwrite GetValue mismatch at index " + to_string(i));

    // Invalid index
    ASSERT(!pt.SetValue<T>(val1, 999), "Invalid SetValue passed");
    ASSERT(pt.GetValue<T>(999) == T{}, "Invalid GetValue did not return default");

    auto stop = high_resolution_clock::now();
    cout << "[BENCHMARK] Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
}

// === Test 3: Name Handling ===
void test_Name_Handling() {
    cout << "\n[TEST 3] Name Handling" << endl;
    auto start = high_resolution_clock::now();

    Point pt(VAR_INT32, "original", 1);
    ASSERT(strcmp(pt.GetName(), "original") == 0, "Initial name incorrect");
    ASSERT(pt.SetName("renamed"), "SetName failed");
    ASSERT(strcmp(pt.GetName(), "renamed") == 0, "Renamed GetName incorrect");

    auto stop = high_resolution_clock::now();
    cout << "[BENCHMARK] Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
}

// === Test 4: Type & FlagMark ===
void test_TypeAndFlag() {
    cout << "\n[TEST 4] Type & FlagMark" << endl;
    auto start = high_resolution_clock::now();

    Point pt(VAR_UINT16, "flags", 1);
    FlagMark f = pt.GetFlagMark();
    ASSERT(pt.GetType() == VAR_UINT16, "GetType mismatch");
    ASSERT(f.VarType == VAR_UINT16, "Flag VarType incorrect");
    ASSERT(f.DataAllocated, "Flag DataAllocated not set");
    ASSERT(f.NameAllocated, "Flag NameAllocated not set");

    auto stop = high_resolution_clock::now();
    cout << "[BENCHMARK] Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
}

// === Test 5: Destructor loop ===
void test_Destructor() {
    cout << "\n[TEST 5] Destructor Loop" << endl;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        Point* p = new Point(VAR_FLOAT, "temp", 10);
        delete p;
    }

    auto stop = high_resolution_clock::now();
    cout << "[BENCHMARK] Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
}

int main() {
    auto global_start = high_resolution_clock::now();

    test_ConstructorDestructor();
    test_SetGet_Value<int8_t>(VAR_INT8, "int8_t", -42, 127);
    test_SetGet_Value<uint16_t>(VAR_UINT16, "uint16_t", 1234, 4321);
    test_SetGet_Value<float>(VAR_FLOAT, "float", 1.23f, 4.56f);
    test_SetGet_Value<double>(VAR_DOUBLE, "double", 1.23, 9.87);
    test_Name_Handling();
    test_TypeAndFlag();
    test_Destructor();

    auto global_stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(global_stop - global_start);

    cout << "\n[SUMMARY] Passed: " << passed << ", Failed: " << failed << endl;
    cout << "[TOTAL TIME] " << duration.count() << " ms\n";

    return failed == 0 ? 0 : 1;
}