#include "test.h"

namespace exst {
    class SystemTests : public AbstractExstTest {
    CPPUNIT_TEST_SUITE(SystemTests);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraintsWithAuxiliaryVariablesSystem);
            CPPUNIT_TEST(testUpdateAssignment);
        CPPUNIT_TEST_SUITE_END();
    private:
    public:

        void testMultipleConstraintsAndNonConstraintsWithAuxiliaryVariablesSystem() {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            extendedStatistics->addRule(body, head);

            extendedStatistics->addId(2, 2);
            extendedStatistics->addId(3, 3);
            extendedStatistics->addId(4, 4);
            extendedStatistics->addId(5, 5);
            extendedStatistics->addId(6, 6);
            extendedStatistics->addId(7, 7);

            Clasp::SymbolTable table;
            table.addUnique(1, NULL);
            table.addUnique(2, NULL);
            table.addUnique(3, " ");
            table.addUnique(4, NULL);
            table.addUnique(5, " ");
            table.addUnique(6, " ");
            table.addUnique(7, " ");
            table.addUnique(8, " ");
            table.addUnique(9, " ");
            table.endInit();

            extendedStatistics->setSymbolTable(table);

            std::streambuf *oldCoutStreamBuf = std::cout.rdbuf();
            std::ostringstream strCout;
            std::cout.rdbuf(strCout.rdbuf());
            extendedStatistics->printParameters();
            std::cout.rdbuf(oldCoutStreamBuf);

            std::stringstream res(strCout.str());
            std::string line;

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(",\"Extended Stats\" : ["), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  [\"Dependency Graph Nodes\" , 8]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"Dependency Graph Edges\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"Incidence Graph Nodes\" , 9]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"Incidence Graph Edges\" , 7]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string("  ,[\"Non Horn Clauses\" , 1]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"Non Dual Horn Clauses\" , 1]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string("  ,[\"max clause size\" , 3]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"max positive clause size\" , 3]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"max negative clause size\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"number of variables that occur as positive literals with auxiliary variables\" , 4]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"number of variables that occur as positive literals without auxiliary variables\" , 4]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"number of variables that occur as negative literals with auxiliary variables\" , 4]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"number of variables that occur as negative literals without auxiliary variables\" , 2]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum positive rule size constraint\" , 1]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum positive rule size non-constraint\" , 3]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"total number of atom occurrences constraint\" , 4]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"total number of atom occurrences non-constraint\" , 5]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum number of occurrences of an atom\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum number of positive occurrences of an atom\" , 2]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum number of negative occurrences of an atom\" , 1]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum size of a non-constraint rule\" , 3]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"maximum size of the head and negative body of a rule\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum size of the head of a rule\" , 1]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"maximum size of the positive body of a non-constraint rule\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum size of the negative body of a rule\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum size of the positive body of a constraint\" , 1]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum size of the negative body of a constraint\" , 2]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"number of atoms that occur in the head\" , 3]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"number of atoms that occur in the positive body\" , 3]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"number of atoms that occur in the negative body\" , 3]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximum number of occurences of a variable\" , 2]"), line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", std::string(
                    "  ,[\"maximum number of occurrences of a variable only head and negative body occurences\" , 1]"),
                                         line);

            std::getline(res, line, '\n');
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses",
                                         std::string("  ,[\"maximal size of the answer set\" , 0]"), line);


        }

        void testUpdateAssignment() {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            extendedStatistics->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            extendedStatistics->addRule(body, head);

            Clasp::Literal l(2, false);
            Clasp::LitVec vec(1, l);
            this->extendedStatistics->updateAssignment(vec);
            incidenceGraphStats->iGraphStats.current_assignment.at(0) == l;
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(SystemTests);
};