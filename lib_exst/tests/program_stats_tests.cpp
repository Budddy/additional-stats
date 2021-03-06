#include "test.h"

namespace exst {
    class ExstTests : public AbstractExstTest {
    CPPUNIT_TEST_SUITE(ExstTests);
            CPPUNIT_TEST(testOneConstraint);
            CPPUNIT_TEST(testOneConstraintAndOneFact);
            CPPUNIT_TEST(testMultipleConstraints);
            CPPUNIT_TEST(testMultipleFacts);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraints);
            CPPUNIT_TEST(testMultipleConstraintsAndNonConstraintsWithAuxiliaryVariables);
        CPPUNIT_TEST_SUITE_END();
    private:
    public:

        void testOneConstraint() {
            std::list<lit_type> bodies;
            bodies.push_back(*new lit_type(2, NEGATIVE));
            bodies.push_back(*new lit_type(3, POSITIVE));
            std::list<lit_type> heads;
            heads.push_back(*new lit_type(4, POSITIVE));
            statsCalculator->addRule(bodies, heads);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);

            Clasp::SymbolTable table;
            table.addUnique(1, " ");
            table.addUnique(2, " ");
            table.addUnique(3, " ");
            table.addUnique(4, " ");
            table.endInit();

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences", (std::size_t) 3,
                                         statsCalculator->generalStatistics.atomOccurences.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Positive", (std::size_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesPositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Negative", (std::size_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testOneConstraintAndOneFact() {
            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(4, POSITIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);

            Clasp::SymbolTable table;
            table.addUnique(1, " ");
            table.addUnique(2, " ");
            table.addUnique(3, " ");
            table.addUnique(4, " ");
            table.endInit();

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences", (std::size_t) 3,
                                         statsCalculator->generalStatistics.atomOccurences.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Positive", (std::size_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesPositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Negative", (std::size_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraints() {
            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, POSITIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, POSITIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            head.push_back(*new lit_type(6, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);


            Clasp::SymbolTable table;
            table.addUnique(1, " ");
            table.addUnique(2, " ");
            table.addUnique(3, " ");
            table.addUnique(4, " ");
            table.addUnique(5, " ");
            table.addUnique(6, " ");
            table.addUnique(7, " ");
            table.endInit();

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 3,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 5,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 3,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 5,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 8,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 6,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 4,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleFacts() {
            std::list<lit_type> body;
            std::list<lit_type> head;
            head.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);


            Clasp::SymbolTable table;
            table.addUnique(1, " ");
            table.addUnique(2, " ");
            table.addUnique(3, " ");
            table.addUnique(4, " ");
            table.addUnique(5, " ");
            table.endInit();

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 1,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 0,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraintsAndNonConstraints() {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);

            Clasp::SymbolTable table;
            table.addUnique(1, " ");
            table.addUnique(2, " ");
            table.addUnique(3, " ");
            table.addUnique(4, " ");
            table.addUnique(5, " ");
            table.addUnique(6, " ");
            table.addUnique(7, " ");
            table.addUnique(8, " ");
            table.addUnique(9, " ");
            table.endInit();

            statsCalculator->setSymbolTable(table);

            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 4,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 5,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numConstraints);
        }

        void testMultipleConstraintsAndNonConstraintsWithAuxiliaryVariables() {

            std::list<lit_type> body;
            std::list<lit_type> head;
            body.push_back(*new lit_type(2, NEGATIVE));
            body.push_back(*new lit_type(3, NEGATIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(4, NEGATIVE));
            body.push_back(*new lit_type(5, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            body.push_back(*new lit_type(5, POSITIVE));
            body.push_back(*new lit_type(6, POSITIVE));
            head.push_back(*new lit_type(7, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(8, POSITIVE));
            statsCalculator->addRule(body, head);

            head.clear();
            body.clear();
            head.push_back(*new lit_type(9, NEGATIVE));
            statsCalculator->addRule(body, head);

            statsCalculator->addId(2, 2);
            statsCalculator->addId(3, 3);
            statsCalculator->addId(4, 4);
            statsCalculator->addId(5, 5);
            statsCalculator->addId(6, 6);
            statsCalculator->addId(7, 7);

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

            statsCalculator->setSymbolTable(table);
            statsCalculator->calculateParameters();

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Non Dual Horn Clauses", (uint32_t) 1,
                                         statsCalculator->generalStatistics.numNonDualHornClauses);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSize);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Positive", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxClauseSizePositive);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Clause Size Negative", (uint32_t) 2,
                                         statsCalculator->generalStatistics.maxClauseSizeNegative);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurences));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Positive", (uint32_t) 2,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesPositive));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Number Atom Occurences Negative", (uint32_t) 1,
                                         maxValue(statsCalculator->generalStatistics.atomOccurencesNegative));
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Negative Variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variableNegative.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Positive Variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variablePositive.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Negative Variables Without auxiliary variables", (std::size_t) 2,
                                         statsCalculator->generalStatistics.variableNegativeWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Positive Variables Without auxiliary variables", (std::size_t) 4,
                                         statsCalculator->generalStatistics.variablePositiveWithoutAuxiliaryVariables.size());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Constraint", (uint32_t) 1,
                                         statsCalculator->generalStatistics.maxPositiveSizeConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Max Positive Rule Size Non Constraint", (uint32_t) 3,
                                         statsCalculator->generalStatistics.maxPositiveRuleSizeNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Constraint", (uint32_t) 4,
                                         statsCalculator->generalStatistics.atomOccurencesConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Atom Occurences Non Constraint", (uint32_t) 5,
                                         statsCalculator->generalStatistics.atomOccurencesNonConstraint);
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of Constraint", (uint32_t) 2,
                                         statsCalculator->generalStatistics.numConstraints);
        }
    };

    CPPUNIT_TEST_SUITE_REGISTRATION(ExstTests);
};