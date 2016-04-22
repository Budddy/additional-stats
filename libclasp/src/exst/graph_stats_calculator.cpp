#include <clasp/exst/graph_stats_calculator.h>
#include <clasp/literal.h>
#include <iostream>
#include <bits/unordered_map.h>
#include <htd/Label.hpp>

namespace exst
{
    void GraphStatsCalculator::addDep(std::vector<uint32_t> dependencies, Clasp::PodVector<Clasp::Var>::type heads,
                                      uint32_t negative)
    {
        //exclude facts (where id is 0)
        if (dependencies.size() == 0)
        {
            return;
        }
        //non horn clause
        if (negative != 0)
        {
            ++numNonHornClauses;
        }
        //normal clause TODO
        if(false){
            ++normalClause;
        }
        //non dual horn clause
        if (negative < dependencies.size() - 1)
        {
            ++numNonDualHornClauses;
        }
        addRuleDependencyGraph(dependencies, heads, negative);
        addRuleIncidenceGraph(dependencies, heads, negative);
    }
    void GraphStatsCalculator::addRuleIncidenceGraph(std::vector<uint32_t> deps,
                                                     Clasp::PodVector<Clasp::Var>::type heads, uint32_t negative)
    {
        htd::id_t rule_Vertex = incidenceGraphStats.minIncidenceGraph.addVertex();

        incidenceGraphStats.ruleVertexMap[incidenceGraphStats.rules] = rule_Vertex;
        uint32_t ruleNumber = incidenceGraphStats.rules;
        incidenceGraphStats.rules++;

        // add body atoms
        for (int i = 0; i < deps.size(); ++i)
        {
            uint32_t dId = deps[i];
            if (incidenceGraphStats.atomVertexMap.count(dId) == 0)
            {
                incidenceGraphStats.atomVertexMap[dId] = incidenceGraphStats.minIncidenceGraph.addVertex();
            }
            bool neg = i < negative;
            incidenceGraphStats.ruleBodyMap[ruleNumber][dId] = neg;
            incidenceGraphStats.bodyRuleMap[dId][ruleNumber] = neg;
        }

        // add head atoms
        for (int i = 0; i < heads.size(); ++i)
        {
            uint32_t hId = heads[i];
            if (incidenceGraphStats.atomVertexMap.count(hId) == 0)
            {
                incidenceGraphStats.atomVertexMap[hId] = incidenceGraphStats.minIncidenceGraph.addVertex();
            }
            incidenceGraphStats.minIncidenceGraph.addEdge(rule_Vertex, incidenceGraphStats.atomVertexMap[hId]);
        }
    }
    void GraphStatsCalculator::addRuleDependencyGraph(std::vector<uint32_t> bodies,
                                                      Clasp::PodVector<Clasp::Var>::type heads, uint32_t negative)
    {
        std::unordered_map<int32_t, htd::id_t> &vertexNodeMap = dependencyGraphStats.atomVertexMap;
        htd::LabeledHypergraph &graph = dependencyGraphStats.dependencyGraph;

        // add body atoms to graph if they are not in it
        for (int i = 0; i < bodies.size(); ++i)
        {
            if (vertexNodeMap.count(bodies[i]) == 0)
            {
                vertexNodeMap[bodies[i]] = graph.addVertex();
            }
        }

        // add head atoms to graph if they are not in it
        for (int i = 0; i < heads.size(); ++i)
        {
            if (vertexNodeMap.count(heads[i]) == 0)
            {
                vertexNodeMap[heads[i]] = graph.addVertex();
            }
        }

        for (int a = 0; a < heads.size(); ++a)
        {
            for (int b = 0; b < bodies.size(); ++b)
            {
                if ((heads[a]) > 1 && dependencyGraphStats.edgeMap[heads[a]].count(bodies[b]) == 0)
                {
                    graph.addEdge(vertexNodeMap[heads[a]], vertexNodeMap[bodies[b]]);
                    dependencyGraphStats.edgeMap[heads[a]][bodies[b]] = heads[a];
                    dependencyGraphStats.edgeMap[bodies[b]][heads[a]] = bodies[b];
                };
            }
        }

        if (heads.size() == 0 || heads.front() == 1)
        {
            for (int a = 0; a < bodies.size(); ++a)
            {
                for (int b = 0; b < bodies.size(); ++b)
                {
                    if ((bodies[a]) != (bodies[b]) && dependencyGraphStats.edgeMap[bodies[a]].count(bodies[b]) == 0)
                    {
                        graph.addEdge(vertexNodeMap[bodies[a]], vertexNodeMap[bodies[b]]);
                        dependencyGraphStats.edgeMap[bodies[a]][bodies[b]] = bodies[a];
                        dependencyGraphStats.edgeMap[bodies[b]][bodies[a]] = bodies[b];
                    };
                }
            }
        }
    }
    void GraphStatsCalculator::addAtomReduct(const Clasp::Literal lit)
    {
        bool neg = true;
        uint32_t atomId = atomIds[lit.var()];
        selectedAtoms[atomId] = neg;
    }
    void GraphStatsCalculator::resetAssignment()
    {
        incidenceGraphStats.incidenceGraphReduct = (incidenceGraphStats.minIncidenceGraph);
        generateReductGraph();
        printIGraphReduct();
        selectedAtoms.clear();
    }
    void GraphStatsCalculator::generateReductGraph()
    {
        for (std::unordered_map<unsigned int, std::map<unsigned int, bool>>::iterator i = incidenceGraphStats.ruleBodyMap.begin();
             i != incidenceGraphStats.ruleBodyMap.end(); i++)
        {
            bool reduce = false;
            //check if rule body is in the reduct
            for (std::map<unsigned int, bool>::iterator body = (*i).second.begin(); body != (*i).second.end(); body++)
            {
                // sign in body is equal to sign in selected atom
                if (selectedAtoms.count((*body).first) > 0 && selectedAtoms[(*body).first] != (*body).second)
                {
                    reduce = true;
                    break;
                }
            }
            //add body to graph if it is in the reduct
            if (!reduce)
            {
                for (std::map<unsigned int, bool>::iterator body = (*i).second.begin();
                     body != (*i).second.end(); body++)
                {
                    incidenceGraphStats.incidenceGraphReduct.addEdge(incidenceGraphStats.ruleVertexMap[(*i).first],
                                                                     incidenceGraphStats.atomVertexMap[(*body).first]);
                }
            }
        }
    }
    void GraphStatsCalculator::addId(uint32 before, uint32 after)
    {
        atomIds[after] = before;
    }
    void GraphStatsCalculator::labelGraph(const Clasp::SymbolTable &symbolTable)
    {
        sTable = &symbolTable;
        labelDepGraph(*sTable);
        labelInzGraph(*sTable, incidenceGraphStats.incidenceGraph);
    }
    void GraphStatsCalculator::labelInzGraph(const Clasp::SymbolTable &symbolTable, htd::LabeledHypergraph &graph)
    {
        std::unordered_map<int32, htd::vertex_t> &litVertexMap = incidenceGraphStats.atomVertexMap;
        std::unordered_map<int32, htd::vertex_t> &ruleVertexMap = incidenceGraphStats.ruleVertexMap;

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++)
        {
            if (litVertexMap[i] == 0)
            {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr)
            {
                name = "unknown";
            } else
            {
                name = sym->name.c_str();
            }

            //name of the atom
            graph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            graph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }

        for (Clasp::SymbolTable::key_type i = 0; i < ruleVertexMap.size(); i++)
        {
            if (ruleVertexMap[i] == 0)
            {
                continue;
            }

            //name of the atom
            std::string name = "rule:";
            name.append(std::to_string(i));
            graph.setVertexLabel("name", ruleVertexMap[i], new htd::Label<std::string>(name.c_str()));

            //id of the atom
            std::string id = "r_";
            id.append(std::to_string(i));
            graph.setVertexLabel("id", ruleVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }
    }
    void GraphStatsCalculator::labelDepGraph(const Clasp::SymbolTable &symbolTable)
    {
        std::unordered_map<int32, htd::vertex_t> &litVertexMap = dependencyGraphStats.atomVertexMap;
        htd::LabeledHypergraph &dependencyGraph = dependencyGraphStats.dependencyGraph;

        for (Clasp::SymbolTable::key_type i = 0; i < litVertexMap.size(); i++)
        {
            if (litVertexMap[i] == 0)
            {
                continue;
            }
            const Clasp::SymbolTable::symbol_type *sym = symbolTable.find(i);

            //atom has no name
            const char *name;
            if (sym == nullptr)
            {
                name = "unknown";
            } else
            {
                name = sym->name.c_str();
            }

            //name of the atom
            dependencyGraph.setVertexLabel("name", litVertexMap[i], new htd::Label<std::string>(name));

            //id of the atom
            std::string id = std::to_string(i);
            dependencyGraph.setVertexLabel("id", litVertexMap[i], new htd::Label<std::string>(id.c_str()));
        }
    }
    void GraphStatsCalculator::printIGraphReduct()
    {
        std::cout << "\n_Incidence Graph Reduct_ \nNodes: ";
        std::cout << incidenceGraphStats.incidenceGraphReduct.vertexCount();
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.incidenceGraphReduct.edgeCount();
        std::cout << "\n";
        /*
        std::cout << "\nIncidence Graph Reduct\n";
        labelInzGraph(*sTable,incidenceGraphStats.incidenceGraphReduct);
        printEdgeList(incidenceGraphStats.incidenceGraphReduct);
        */
    }
    void GraphStatsCalculator::printDepGraph()
    {
        std::cout << "\n_Dependency Graph_ \nNodes: ";
        std::cout << dependencyGraphStats.dependencyGraph.vertexCount();
        std::cout << "\nEdges: ";
        std::cout << dependencyGraphStats.dependencyGraph.edgeCount();
        /*
        std::cout << "\nDependency Graph\n";
        printEdgeList(dependencyGraphStats.dependencyGraph);
         */
    }
    void GraphStatsCalculator::printIncidenceGraph()
    {
        std::cout << "_Incidence Graph_ \nNodes: ";
        std::cout << incidenceGraphStats.incidenceGraph.vertexCount();
        std::cout << "\nEdges: ";
        std::cout << incidenceGraphStats.incidenceGraph.edgeCount();
        /*
        std::cout << "\nIncidence Graph Program\n";
        printEdgeList(incidenceGraphStats.incidenceGraph);
         */
    }
    void GraphStatsCalculator::printEdgeList(htd::LabeledHypergraph &graph)
    {
        htd::ConstCollection<htd::vertex_t> vertices = graph.vertices();

        for (int a = 0; a < vertices.size(); a++)
        {
            const htd::vertex_t &vertex = vertices[a];
            const htd::ILabel &nameLable = graph.vertexLabel("name", vertex);
            htd::ConstCollection<htd::Hyperedge> edges = graph.hyperedges(vertex);
            graph.vertexLabel("id", vertex).print(std::cout);
            printf(": ");
            nameLable.print(std::cout);
            printf("\n");
            for (int b = 0; b < edges.size(); b++)
            {
                htd::vertex_t v = edges[b][0] == vertex ? edges[b][1] : edges[b][0];
                printf(" -> ");
                graph.vertexLabel("id", v).print(std::cout);
                printf(": ");
                graph.vertexLabel("name", v).print(std::cout);
                printf("\n");
            }
        }

        std::flush(std::cout);
    }
    void GraphStatsCalculator::buildIncidenzeGraph()
    {
        this->incidenceGraphStats.incidenceGraph = this->incidenceGraphStats.minIncidenceGraph;
        for (std::unordered_map<unsigned int, std::map<unsigned int, bool>>::iterator i = incidenceGraphStats.ruleBodyMap.begin();
             i != incidenceGraphStats.ruleBodyMap.end(); i++)
        {
            for (std::map<unsigned int, bool>::iterator body = (*i).second.begin(); body != (*i).second.end(); body++)
            {
                incidenceGraphStats.incidenceGraph.addEdge(incidenceGraphStats.ruleVertexMap[(*i).first],
                                                           incidenceGraphStats.atomVertexMap[(*body).first]);
            }
        }
    }
    void GraphStatsCalculator::printExtendedStats() {
        printDepGraph();
        printIncidenceGraph();
        std::cout << "\n";
        std::cout << "Non Horn Clauses: " << this->numNonHornClauses << "\n";
        //std::cout << "Non Normal Clauses: " << this->normalClause << "\n";
        std::cout << "Non Dual Horn Clauses: " << this->numNonDualHornClauses << "\n";
    }

}