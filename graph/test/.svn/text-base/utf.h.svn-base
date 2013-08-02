#ifndef UNITTEST_H
#define UNITTEST_H

#include <iostream>

#define utf_test(t1,t2) _test(t1,t2,__LINE__,__FILE__)
#define utf_test_approx(t1,t2) _test_approx(t1,t2,__LINE__,__FILE__)

class UnitTest {
private:
  int tests_failed;
  int tests_passed;
  int total_tests_failed;
  int total_tests_passed;
  std::string test_set_name;
  std::string current_description;
  std::ostream *out;

public:

  UnitTest(std::string test_set_name_in,std::ostream *out_in = &std::cerr) : tests_failed(0),
                                                                          tests_passed(0),
                                                                          total_tests_failed(0),
                                                                          total_tests_passed(0),
                                                                          current_description(),
                                                                          test_set_name(test_set_name_in),
									  out(out_in) {
    (*out) << "*** Test set  : " << test_set_name << std::endl;
  }

  void begin_test_set(std::string description) {
    current_description = description;
    tests_failed = 0;
    tests_passed = 0;
    (*out) << "****** Testing: " << current_description << std::endl;
  }

  void end_test_set() {
    (*out) << "****** Test   : " << current_description << " complete, ";
    (*out) << "passed " << tests_passed << ", failed " << tests_failed << "." << std::endl;
  }

  template<class _TestType>
  void _test(_TestType t1,_TestType t2,int linenumber,const char *current_file) {
    bool test_result = (t1 == t2);

    if(!test_result) {
        (*out) << "****** FAILED : " << current_file << "," << linenumber;
	(*out) << ": " << t1 << " is not equal to " << t2 << std::endl;
        total_tests_failed++;
        tests_failed++;
    } else { tests_passed++; total_tests_passed++; }
  }

  template<class _TestType>
  void _test_approx(_TestType t1,_TestType t2,_TestType max_error,int linenumber,const char *current_file) {
    _TestType difference = t1-t2;
    if(difference < 0) difference = 0-difference;

    if(difference > max_error) {
      (*out) << "****** FAILED : " << current_file << "," << linenumber;
      (*out) << ": " << t1 << " is not equal to " << t2 << " within difference of " << max_error << std::endl;
      tests_failed++;
      total_tests_failed++;
    } else { tests_passed++; total_tests_passed++; }
  }

  void test_report() {
    (*out) << "*** Test set  : " << test_set_name << " complete, ";
    (*out) << "passed " << total_tests_passed;
    (*out) << " failed " << total_tests_failed << "." << std::endl;
    if(total_tests_failed != 0) (*out) << "*** TEST FAILED!" << std::endl;
  }
};

#endif
