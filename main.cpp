// EECS 348 Lab 11: Debugging and Testing
//
//   # EECS 348 Lab 11: Debugging and Testing
//
//   ## 1. Is there a fault in the program? If so, locate it and explain.
//   Yes, there is a fault. The fault is in the line with `for (int i = 1; i < attendance_records.size(); ++i)`. The loop starts with i = 1, so the first lecture of the attendance record gets skipped. This can be fixed by using i = 0.
//
//   ## 2. Define a test case that does not execute the fault.
//   There is no test case that doesn't execute the fault. Since it is in the for loop's initialization, it will always run, starting at i = 1. It will run for every call of the function, even if an empty vector is used.
//
//   ## 3. Define a test case that executes the fault but does not result in an error state.
//   Technically such a case is not possible, but if the state is only determined by absent_count, then if the first lecture is attended no error would occur, because the number of absences would remain the same whether the loop started at i = 1 or i = 0. For example, if the array looked like {1,0,1,0,1,1,1,1,1,1}, the number of absences would be the same whether lecture 1 is included or not.
//
//   ## 4. Define a test case that results in an error state but not a failure.
//   Say the array was {0,0,0,0,1,1,1,1,1,1}. The expected absence count is 4, but with the fault the count will be 3. However, both results lead to the correct output of the student failing the class, so no failure in the program occurred, because the output is the expected output overall.
//
//   ## 5. Define a test case that results in failure.
//   Say the array was {0,0,0,1,1,1,1,1,1,1}. The expected absence count is 3, but with the fault the count will be 2. This results in a failure because the output should be a fail but will instead be a pass, because the absence count will not reach three or more with the fault.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Function under test (contains the fault, intentionally left as-is)
bool fail_lecture(const vector<int>& attendance_records, int& absent_count_out) {
    int absent_count = 0;
    for (int i = 1; i < attendance_records.size(); ++i) {   // FAULT: should be i = 0
        absent_count += attendance_records[i] == 0;
    }
    absent_count_out = absent_count;   // exposed only so the test can print it
    return absent_count >= 3;
}

struct TestCase {
    string name;
    vector<int> record;
};

static int expected_count(const vector<int>& r) {
    int c = 0;
    for (int v : r) if (v == 0) ++c;
    return c;
}

static void print_vec(const vector<int>& r) {
    cout << "[";
    for (size_t i = 0; i < r.size(); ++i) cout << r[i] << (i + 1 < r.size() ? " " : "");
    cout << "]";
}

static void run(const TestCase& t) {
    int actual_count;
    bool actual = fail_lecture(t.record, actual_count);
    int exp_count = expected_count(t.record);
    bool expected = exp_count >= 3;

    cout << "=== " << t.name << " ===\n";
    cout << "Attendance record : "; print_vec(t.record); cout << "\n";
    cout << "Expected absences : " << exp_count << "\n";
    cout << "Actual absences   : " << actual_count << "\n";
    cout << "Expected output   : " << (expected ? "true" : "false") << "\n";
    cout << "Actual output     : " << (actual ? "true" : "false") << "\n";
    cout << "Error state?      : " << (exp_count != actual_count ? "YES" : "no (absent_count correct)") << "\n";
    cout << "Failure?          : " << (expected != actual ? "YES" : "no") << "\n\n";
}

int main() {
    cout << "Q1: Fault exists at 'for (int i = 1; ...)' (should start at 0).\n\n";
    cout << "Q2: No such test case - the loop initialization runs on every call.\n\n";
    cout << "Q3: Technically not possible, but if the state is only absent_count,\n"
            "    a record with the first lecture attended leaves the count correct:\n\n";

    run({"Q3 (fault executed, absent_count unaffected)",
         {1,0,1,0,1,1,1,1,1,1}});
    run({"Q4 (error state, no failure)", {0,0,0,0,1,1,1,1,1,1}});
    run({"Q5 (failure)",                 {0,0,0,1,1,1,1,1,1,1}});
    return 0;
}
