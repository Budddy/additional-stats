#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <clasp/dependency_graph.h>
#include <clasp/literal.h>
#include <clasp/shared_context.h>
#include <clasp/logic_program.h>
#include <htd/LabeledGraph.hpp>
#include <bits/stl_list.h>
#include <bits/stl_pair.h>
#include <htd/Label.hpp>

namespace exst {
    class GraphStatsCalculator {

    public:
        static GraphStatsCalculator &getInstance() {
            static GraphStatsCalculator calc;
            return calc;
        }
        void printMatrix(bool printAll);
        void printEdgeList();
        void addDependency(std::list<uint32> deps, Clasp::VarVec heads);
        void lableGraph(const Clasp::SymbolTable &symbolTable);

    private:
        htd::LabeledGraph graph;
        std::map<uint32, htd::vertex_t> vertexNodeMap;
        ulong maxLableLength;
        ulong maxIdLength;

        GraphStatsCalculator() { };
        GraphStatsCalculator(GraphStatsCalculator const &);
        void operator=(GraphStatsCalculator const &);
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
