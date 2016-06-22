#include <exst/extended_stats_calculator.h>
#include <iostream>
#include <fstream>

namespace exst
{
    void StatsCalculator::parseRule(std::list<lit_type> body, std::list<lit_type> head)
    {
        //rules
        numRules++;

        // num clauses
        if (body.size() > 0)
            numClauses++;

        // num facts
        if (body.size() == 0)
            numFacts++;

        uint32_t negative = 0;
        std::list<exst::lit_type>::iterator i;
        for (i = body.begin(); i != body.end(); i++)
        {
            if ((*i).s == -1)
            {
                negative++;
            }
        };

        if (head.front().id == 1)
        {
            maxPositiveRuleSizeConstraint = (uint32_t) (maxPositiveRuleSizeConstraint < body.size() - negative ?
                                                        body.size() - negative : maxPositiveRuleSizeConstraint);
            constraint++;
            atomOccurencesConstraint += head.size();
            atomOccurencesConstraint += body.size();
        } else
        {
            maxPositiveRuleSizeNonConstraint = (uint32_t) (maxPositiveRuleSizeNonConstraint <
                                                           body.size() - negative + head.size() ? body.size() -
                                                                                                  negative + head.size()
                                                                                                : maxPositiveRuleSizeNonConstraint);
            atomOccurencesNonConstraint += head.size();
            atomOccurencesNonConstraint += body.size();
        }
        //non horn clause
        if (negative != 0)
            ++numNonHornClauses;

        //non dual horn clause
        if (negative < body.size() - 1)
            ++numNonDualHornClauses;

        //clause size
        maxClauseSize = (maxClauseSize < body.size() + head.size() ? body.size() + head.size() : maxClauseSize);
        //positive clause size
        maxClauseSizePositive = (maxClauseSizePositive < body.size() - negative + head.size() ? body.size() - negative +
                                                                                                head.size()
                                                                                              : maxClauseSizePositive);
        //negative clause size
        maxClauseSizeNegative = maxClauseSizeNegative < negative ? negative : maxClauseSizeNegative;

        //get variables
        variableOccurrences(body, head);

        //atom occurrences
        countAtomOccurences(body, head);

        //number of variables that occur in positive/negative literals
        parseVariableLiteral(body, head);

        graphStatsCalculator.dependencyGraphStats.addRuleDependencyGraph(body, head);
        graphStatsCalculator.incidenceGraphStats.addRuleIncidenceGraph(body, head);
    }

    void StatsCalculator::countAtomOccurences(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                atomOccurencesNegative[id]++;
            } else
            {
                atomOccurencesPositive[id]++;
            }
            atomOccurences[id]++;
        }
        for (it = head.begin(); it != head.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                atomOccurencesNegative[id]++;
            } else
            {
                atomOccurencesPositive[id]++;
            }
            atomOccurencesPositive[id]++;
        }
    }

    void StatsCalculator::variableOccurrences(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                variableNegative[id] = true;
            } else
            {
                variablePositive[id] = true;
            }
        }
        for (it = head.begin(); it != head.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                variableNegative[id] = true;
            } else
            {
                variablePositive[id] = true;
            }
        }
    }

    void StatsCalculator::addId(uint32_t before, uint32_t after)
    {
        if (after == 0)
        {
            variableNegative.erase(variableNegative.find(before));
            variablePositive.erase(variablePositive.find(before));
            variableNegativeWithoutHelper.erase(variableNegative.find(before));
            variablePositiveWithoutHelper.erase(variablePositive.find(before));
        }
        atomIds[after] = before;
    }

    void StatsCalculator::printExtendedStats()
    {
        graphStatsCalculator.dependencyGraphStats.printDepGraph();
        graphStatsCalculator.incidenceGraphStats.printIncidenceGraph();
        std::cout << "\nNon Horn Clauses: " << numNonHornClauses << "\n";
        std::cout << "Non Dual Horn Clauses: " << numNonDualHornClauses << "\n";
        std::cout << "Clauses: " << numClauses << "\n";
        std::cout << "Facts: " << numFacts << "\n";
        std::cout << "Rules: " << numRules << "\n";
        std::cout << "Constraints: " << constraint << "\n";

        //maximum weight of the minimal model
        std::cout << "\nmaximum weight of the minimal model: " << maxWeightMinModel << "\n";

        //maximum clause size
        std::cout << "\nmax clause size: " << maxClauseSize << "\n";

        //maximum positive/negative clause size, i.e., only positive/negative literals are counted
        std::cout << "max positive clause size: " << maxClauseSizePositive << "\n";
        std::cout << "max negative clause size: " << maxClauseSizeNegative << "\n";

        //number of variables that occur as positive/negative literals
        std::cout << "\nnumber of variables that occur as positive literals: " << "\n";
        std::cout << "with helpers: " << variablePositive.size() << " without helpers: " <<
        variablePositiveWithoutHelper.size() << "\n";
        std::cout << "number of variables that occur as negative literals: " << "\n";
        std::cout << "with helpers: " << variableNegative.size() << " without helpers: " <<
        variableNegativeWithoutHelper.size() << "\n";

        //maximum positive rule size (constraint/non-constraint)
        std::cout << "\nmaximum positive rule size constraint: " << maxPositiveRuleSizeConstraint << "\n";
        std::cout << "maximum positive rule size non-constraint: " << maxPositiveRuleSizeNonConstraint << "\n";

        //total number of atom occurrences in the program (constraints/non-constraint)
        std::cout << "\ntotal number of atom occurrences constraint: " << atomOccurencesConstraint << "\n";
        std::cout << "total number of atom occurrences non-constraint: " << atomOccurencesNonConstraint << "\n";

        //maximum number of occurrences of an atom
        std::cout << "\nmaximum number of occurrences of an atom: " << maxValue(atomOccurences) << "\n";

        //maximum number of positive occurrences of an atom
        std::cout << "maximum number of positive occurrences of an atom: " << maxValue(atomOccurencesPositive) << "\n";

        //maximum number of negative occurrences of an atom
        std::cout << "maximum number of negative occurrences of an atom: " << maxValue(atomOccurencesNegative) << "\n";

        //TODO
        /*std::ofstream dfile;
        dfile.open("dgraph.txt", std::ios::out);
        dfile << getDIMACS(graphStatsCalculator.dependencyGraphStats.dependencyGraph,
                           graphStatsCalculator.dependencyGraphStats.edgecount);

        std::ofstream ifile;
        ifile.open("igraph.txt", std::ios::out);
        ifile << getDIMACS(graphStatsCalculator.incidenceGraphStats.incidenceGraph,
                           graphStatsCalculator.incidenceGraphStats.edgecount);*/

        std::flush(std::cout);
    }

    void StatsCalculator::parseVariableLiteral(std::list<lit_type> body, std::list<lit_type> head)
    {
        std::list<exst::lit_type>::iterator it;
        for (it = body.begin(); it != body.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                variableNegative[id] = true;
                variableNegativeWithoutHelper[id] = true;
            } else
            {
                variablePositive[id] = true;
                variablePositiveWithoutHelper[id] = true;
            }
        }
        for (it = head.begin(); it != head.end(); it++)
        {
            uint32_t id = it->id;
            if (it->s == -1)
            {
                variableNegative[id] = true;
                variableNegativeWithoutHelper[id] = true;
            } else
            {
                variablePositive[id] = true;
                variablePositiveWithoutHelper[id] = true;
            }
        }
    }

    void StatsCalculator::setSymbolTable(std::unordered_map<uint32_t, const char *> &table)
    {
        this->sTable = &table;
        std::list<unsigned int> rem;
        std::unordered_map<unsigned int, bool>::iterator it;
        for (it = variablePositiveWithoutHelper.begin(); it != variablePositiveWithoutHelper.end(); it++)
        {
            if (table.count(it->first) == 0)
            {
                rem.push_back((*it).first);
            }
        }
        while (rem.size() > 0)
        {
            variablePositiveWithoutHelper.erase(rem.front());
            rem.pop_front();
        }
        for (it = variableNegativeWithoutHelper.begin(); it != variableNegativeWithoutHelper.end(); it++)
        {
            if (table.count((*it).first) == 0)
            {
                rem.push_back((*it).first);
            }
        }
        while (rem.size() > 0)
        {
            variableNegativeWithoutHelper.erase(rem.front());
            rem.pop_front();
        }
    }
}