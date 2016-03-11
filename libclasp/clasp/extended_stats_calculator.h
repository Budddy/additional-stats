#ifndef CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H
#define CLASP_ABSTRACTEXTENDEDSTATSCALCULATOR_H

#include <clasp/literal.h>
#include <clasp/shared_context.h>
//#include <clasp/logic_program.h>
#include <htd/LabeledHypergraph.hpp>
#include <htd/Label.hpp>

namespace exst {

    class IncidenceGraphStats{
    public:
        htd::LabeledHypergraph& getGraph(){return inzidenceGraph;};
        std::map<int32, htd::vertex_t>& getLitVertexMap(){return litVertexMap;};
        std::map<int32, htd::vertex_t>& getRuleVertexMap(){return ruleVertexMap;};
        int& getRuleCount(){return rules;};
        void setCopy(const htd::LabeledHypergraph & graph){
            copyIGraph = graph;
        }
        htd::LabeledHypergraph & getCopy(){
            return copyIGraph;
        }

    private:
        //inzidence graph
        htd::LabeledHypergraph inzidenceGraph;
        htd::LabeledHypergraph copyIGraph;
        //rulecount
        int rules=0;
        //mapping from graph vertices to literals
        std::map<int32, htd::vertex_t> litVertexMap;
        std::map<int32, htd::vertex_t> ruleVertexMap;

    };

    class DependencyGraphStats{
    public:
        htd::LabeledHypergraph& getGraph(){return dependencyGraph;};
        std::map<int32, htd::vertex_t>& getLitVertexMap(){return litVertexMap;};
    private:
        //dependency graph
        htd::LabeledHypergraph dependencyGraph;
        //mapping from graph vertices to literals
        std::map<int32, htd::vertex_t> litVertexMap;
    };

    class GraphStatsCalculator {
    public:

        /*
         * returns the single instace of the GraphStatsCalculator
         */
        static GraphStatsCalculator &getInstance() {
            static GraphStatsCalculator calc;
            return calc;
        }

        /*
         * prints the dependency graph as edge list
         */
        void printEdgeList(htd::LabeledHypergraph graph);

        /*
         * adds a dependency to the dependency graph
         */
        void addDep(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative);

        /*
         * adds the atom lables to the graph
         */
        void lableGraph(const Clasp::SymbolTable &symbolTable);

        /*
         * adds a literal set during solving
         */
        void addAtomReduct(const Clasp::Literal lit);
        void printDepGraph();
        void printIncidenceGraph();
        void printIGraphReduct();
        void addId(uint32 before, uint32 after);

        void resetAssignment();
    private:
        IncidenceGraphStats istats;
        DependencyGraphStats dstats;
        //selected literals of current assignment
        std::vector<int32> selectedLits;
        //used for matching literal ids before and after pre processing
        std::map<int32, uint32> litIds;

        //private constructors for singleton
        GraphStatsCalculator() { };
        void addRuleDependencyGraph(std::vector<uint32> deps, Clasp::VarVec heads, uint32 negative);
        void addRuleIncidenceGraph(std::vector<uint32> vector, Clasp::VarVec pod_vector, uint32 negative);
        void lableDepGraph(const Clasp::SymbolTable &symbolTable);
        void lableInzGraph(const Clasp::SymbolTable &symbolTable);
    };
}

#endif //CLASP_EXTENDEDSTATSCALCULATOR_H
