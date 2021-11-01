#pragma once

#include <algorithm>
using std::sort;
using std::random_shuffle;

#include <fstream>
using std::ifstream;
using std::getline;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <unordered_map>
using std::unordered_map;

#include <vector>
using std::vector;

#include <utility>
using std::pair;

#include <iterator>
using std::iterator;
using std::forward_iterator_tag;

#include <queue>
using std::queue;
using std::priority_queue;

#include <math.h>
using std::stoll;

#include <limits>
using std::numeric_limits;

/**
 * Repesents cryptocurrency flow as a directed graph, with nodes
 * representing addresses and edges representing transactions.
 */
class Graph {

private: // constants

    static const char DELIMITER_ = ',';      // Expected delimiter in input file.
    static const int SATOSHI_ = 100000000;   // BTC to satoshi conversion.

public:

    /**
     * @brief Build graph structure from file.
     * 
     * @param filepath Location of file.
     * @throws Exception if file location or structure is invalid.
     */
    Graph(const string& filepath);

    /**
     * @brief Convert amount formatted in satoshis into BTC.
     * 
     * @param satoshi BTC amount formatted in satoshis.
     * @return double BTC amount.
     */
    static double convertSatoshiToBTC(long long satoshi);

    /**
     * @brief Add sender and recipient as nodes and 
     * edge as transfer of BTC from transaction to graph.
     * 
     * @param senderAddress    Address of sender in BTC blockchain.
     * @param amount           Amount of BTC in satoshis.
     * @param recipientAddress Address of recipient in BTC blockchain.
     */
    void addTransaction(const string& senderAddress, long long amount, const string& recipientAddress);

    /**
     * @brief Checks if specified address exists in graph.
     * 
     * @param address Address to look for.
     * @return bool   True if address exists, false otherwise.
     */
    bool addressExists(const string& address) const;

    /**
     * @brief Amount of BTC transacted between 
     * specified sender and recipient.
     * 
     * @param senderAddress    Address of sender.
     * @param recipientAddress Address of recipient.
     * @return long long       Transaction amount, or 0 
     *                         if there is no transaction.
     */
    long long getTransactionAmount(const string& senderAddress, const string& recipientAddress) const;

    /**
     * @brief Breadth First Search (BFS) of graph.
     * 
     * @return vector<string> Vector of addresses of nodes in BFS order.
     */
    vector<string> BFS() const;

    /**
     * @brief Get the most likely path of BTC transacted 
     * from sender to recipient in Bitcoin graph.
     * 
     * @param senderAddress    Sender address in Bitcoin blockchain.
     * @param recipientAddress Recipient address in Bitcoin blockchain.
     * @return vector<string>  List of BTC addresses in order of path 
     *                         from sender address to recipient address.
     */
    vector<string> mostLikelyPath(const string& senderAddress, const string& recipientAddress) const;

    /**
     * @brief Get addresses in graph ordered by centrality
     * 
     * @param  samplingSize 
     * @return vector<pair<string, unsigned int>> List of addresses with their 
     *                                            centrality score ordered by centrality.
     */
    vector<pair<string, unsigned int>> getAddressesByCentrality(size_t samplingSize = 0) const;

    /**
     * @brief Get the balance of BTC address based on 
     * transactions inputted into graph.
     * 
     * @param address    Address to get balance of. 
     * @return long long Balance of address in graph.
     * 
     * Balance is not accurate if whole Bitcoin 
     * blockchain is not inputted into graph.
     * 
     * Sums inbound transaction amounts minus
     * outbound transactions amounts. 
     */
    long long getAddressBalance(const string& address) const;

    /**
     * @brief Get the number of addresses
     * currently in the graph.
     * 
     * @return size_t Size of node list.
     */
    size_t getAddressCount() const;

    /**
     * @brief Get the number of transactions
     * currently in the graph.
     * 
     * @return size_t Size of edge list.
     */
    size_t getTransactionCount() const;

private:

    struct Edge {
        double weight; // weight stored as 1 / transaction amount.
        size_t senderIdx;
        size_t recipientIdx;

        Edge(size_t senderIdx, long long amount, size_t recipientIdx) : 
            weight(1.0 / amount), senderIdx(senderIdx), recipientIdx(recipientIdx) { }
    };

    struct Node {
        string address;
        size_t index;

        // List of edge indexes
        vector<size_t> outbound;
        vector<size_t> inbound;
    
        Node(const string& address, size_t index) : address(address), index(index) { }
    };

    /**
     * @brief Helper function for traversing component in graph in BFS order.
     * 
     * @param startNodeIdx Index of starter node in component in @a nodes_ list.
     * @param addresses    List of addresses traversed in BFS order.
     * @param visitedNodes List indicating visited nodes in graph.
     */
    void _BFS(size_t startNodeIdx, vector<string>& addresses, vector<bool>& visitedNodes) const;

    /**
     * @brief Get all shortest paths from source node 
     * using Dijkstra's algorithm.
     * 
     * @param nodeIdx              Source node index.
     * @return vector<const Node*> List of node indexes containing 
     *                             predecessors in paths from source node.
     */
    vector<const Node*> _dijkstraSSSP(size_t nodeIdx) const;

    /**
     * @brief Get the Betweeness Centrality of nodes in the graph.
     * 
     * @param samplingSize Amount of nodes to sample from, 
     *                     a value of 0 runs over all nodes.
     * @return vector<int> Vector that contains the centrality 
     *                     of all nodes from sampling.
     */
    vector<int> _getBetweenessCentrality(size_t samplingSize) const;

    /**
     * @brief Get the original transaction amount in satoshis from edge.
     * 
     * @param edgeIdx    Edge to get weight from.
     * @return long long Transaction amount in satoshis.
     */
    long long _getEdgeTransactionAmount(size_t edgeIdx) const;

    /**
     * @brief Adds node to address with the given 
     * BTC address and returns node index.
     * 
     * @param address Address of node.
     * @return        Index of node in @a nodes_ list. 
     */
    size_t _addNode(const string& address);

    /**
     * @brief Adds directed edge from sender to recipient with transaction 
     * amount and returns edge index in @a edges_ list.
     * 
     * @param senderIdx    Index of sender Node in @a nodes_ list. 
     * @param amount       Total transaction amount between.
     * @param recipientIdx Index of recipient Node @a nodes_ list.
     * @return size_t      Index of edge in @a edges_ list.
     * 
     * Only one directed edge from sender to recipient 
     * is allowed on the graph.
     */
    size_t _addEdge(size_t senderIdx, long long amount, size_t recipientIdx);

    /**
     * @brief Split a string by a char into vector.
     * 
     * @param str             The string to split up.
     * @param delimeter       The char that determines where to 
     *                        split up @a str.
     * @return vector<string> List of strings split up 
     *                        by @a delimiter in @a str.
     */
    vector<string> _splitString(const string& str, char delimeter = DELIMITER_) const;

    vector<Node> nodes_;
    vector<Edge> edges_;
    
    // Map, mapping BTC Addresses to Node indexes in nodes_ list.
    unordered_map<string, size_t> node_dict_;

};