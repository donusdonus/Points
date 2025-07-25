#include <stdio.h>
#include <assert.h>
#include "Point.h"

void printAssignPath(Point& p) {
    printf("Assigned: %s | Type: %s | Size: %u bytes | Elements: %u\n",
        p.GetName(), p.GetTypeInfo().name,
        (unsigned)p.GetByteSize(), (unsigned)p.Count());
}

void runTests() {
    printf("=== Running Unit Tests for Point.h ===\n");

    // Test 1: Basic name assignment and retrieval
    Point p1(INT8_T, "Temp", 1);
    assert(strcmp(p1.GetName(), "Temp") == 0); // line 1
    printAssignPath(p1);
    printf("[PASS] Test 1: Set/Get Name\n");

    // Test 2: SetValue / GetValue
    assert(p1.SetValue<int8_t>(42));
    assert(p1.GetValue<int8_t>() == 42); // line 2
    printf("[PASS] Test 2: Set/Get Value\n");

    // Test 3: Array set and get
    int16_t arr[3] = {100, 200, 300};
    Point p2(INT16_T, "Pressure", arr, 3);
    assert(p2.GetValue<int16_t>(0) == 100);
    assert(p2.GetValue<int16_t>(1) == 200); // line 3
    assert(p2.GetValue<int16_t>(2) == 300);
    printAssignPath(p2);
    printf("[PASS] Test 3: Set/Get Array\n");

    // Test 4: ClearValue
    p2.ClearValue();
    assert(p2.GetValue<int16_t>(0) == 0); // line 4
    printf("[PASS] Test 4: ClearValue\n");

    // Test 5: Group behavior
    Point group(GROUP_T, "MyGroup", 1);
    assert(group.AddSlot(p1));
    assert(group.AddSlot(p2));
    Point* f = group.FindSlot("Temp");
    assert(f != NULL);
    assert(strcmp(f->GetName(), "Temp") == 0); // line 5
    assert(group.DeleteSlot(f));
    assert(group.FindSlot("Temp") == NULL); // line 6
    printf("[PASS] Test 5: Group Add/Find/Delete\n");

    // Test 6: TypeInfo
    TypeInfo info = p1.GetTypeInfo();
    assert(info.size == sizeof(int8_t));
    assert(info.canWrite); // line 7
    printf("[PASS] Test 6: TypeInfo\n");

    printf("=== ALL TESTS PASSED ===\n");

}

int main()
{
    runTests();

    return 0;
}