#include <iostream>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/Asserter.h>
#include "calc.h"

class Test : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(testHelloWorld);
  CPPUNIT_TEST(testSum);
  CPPUNIT_TEST(testOp);
  CPPUNIT_TEST(testBrackets);
  CPPUNIT_TEST(testThrow);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {}

protected:
  void testHelloWorld(void) {
    system("echo 1+2 | ./final");
  }

  void testSum()
  {
      ExtendedExpressionParser parser;

      auto expr = parser.parse("1+2");

      CPPUNIT_ASSERT_DOUBLES_EQUAL( expr->evaluate(), 3.0, 1e-6 );
  }

  void testOp()
  {
      ExtendedExpressionParser parser;

      auto expr = parser.parse("1+2*3");

      CPPUNIT_ASSERT_DOUBLES_EQUAL( expr->evaluate(), 7.0, 1e-6 );
  }

  void testBrackets()
  {
      ExtendedExpressionParser parser;

      auto expr = parser.parse("(1+2)*3");

      CPPUNIT_ASSERT_DOUBLES_EQUAL( expr->evaluate(), 9.0, 1e-6 );
  }

  void testThrow()
  {
      ExtendedExpressionParser parser;

      CPPUNIT_ASSERT_THROW( parser.parse("1***3");, std::runtime_error );
  }

};


CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main()

{
  CPPUNIT_NS::TestResult controller;

  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener(&progress);

  CPPUNIT_NS::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  runner.run(controller);

  return result.wasSuccessful() ? 0 : 1;
}
