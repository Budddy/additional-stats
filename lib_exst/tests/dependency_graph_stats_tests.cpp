#include "test.h"

namespace exst
{
    class DependencyTests : public AbstractExstTest
    {
    CPPUNIT_TEST_SUITE(DependencyTests);
            CPPUNIT_TEST(testaddRule);
            CPPUNIT_TEST(testgetStatistics);
            CPPUNIT_TEST(testgetAdditionalStatistics);
        CPPUNIT_TEST_SUITE_END();
    private:
    public:
        void setUp()
        {
            AbstractExstTest::setUp();
        }

        void tearDown()
        {
            AbstractExstTest::tearDown();
        }

        void testaddRule()
        {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 0,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 2, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 0,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 4, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 6, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 7, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 8, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 2,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 8, dependencyGraphStats->getDependencyGraph().size());

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, POSITIVE));
            body.push_back(*new lit_type(4, NEGATIVE));
            head.push_back(*new lit_type(3, POSITIVE));
            dependencyGraphStats->addRule(body, head);

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge Count", (uint32_t) 4,
                                         edgeCount(dependencyGraphStats->getDependencyGraph()));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Node Count", (size_t) 8, dependencyGraphStats->getDependencyGraph().size());
        }

        void testgetStatistics()
        {};

        void testgetAdditionalStatistics()
        {};
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(DependencyTests);
};