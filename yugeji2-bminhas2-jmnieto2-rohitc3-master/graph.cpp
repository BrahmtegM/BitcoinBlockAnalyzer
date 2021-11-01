#include "graph.h"

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

Graph::Graph(const string& filepath) {
    ifstream data(filepath);

    if (data.is_open()) {
        string line;

        while (getline(data, line)) {
            // Get fields (in order: sender, tx amount, receiver) 
            // by splitting current line by commas
            vector<string> fields = _splitString(line);

            try {
                const string& senderAddress = fields.at(0);
                const string& amountStr = fields.at(1);
                const string& recipientAddress = fields.at(2);

                long long amount = stoll(amountStr, NULL, 10);

                addTransaction(senderAddress, amount, recipientAddress);
            } catch (...) {
                throw string("Error loading transactions from file \"" + filepath + "\"");
            }
        }
    } else {
        throw string("File \"" + filepath + "\" does not exist!");
    }
}

double Graph::convertSatoshiToBTC(long long satoshi) {
    return (double) satoshi / SATOSHI_;
}

void Graph::addTransaction(const string& senderAddress, long long amount, const string& recipientAddress) {
    size_t senderIdx = _addNode(senderAddress);
    size_t recipientIdx = _addNode(recipientAddress);
    _addEdge(senderIdx, amount, recipientIdx);
}

bool Graph::addressExists(const string& address) const {
    return node_dict_.count(address);
}

long long Graph::getTransactionAmount(const string& senderAddress, const string& recipientAddress) const {
    if (!addressExists(senderAddress) || !addressExists(recipientAddress))
        return 0;
    
    size_t senderIdx = node_dict_.at(senderAddress);
    size_t recipientIdx = node_dict_.at(recipientAddress);

    const vector<size_t>& senderOutbounds = nodes_[senderIdx].outbound;

    // Get edge directed to recipient
    for (size_t edgeIdx : senderOutbounds) {
        const Edge& edge = edges_[edgeIdx];
        if (edge.recipientIdx == recipientIdx) {
            return _getEdgeTransactionAmount(edgeIdx);
        }
    }

    // Return 0 if there exists no edge to recipient
    return 0;
}

vector<string> Graph::BFS() const {
    vector<string> addresses;
    vector<bool> visitedNodes(nodes_.size());
    
    // Run BFS on every component until all nodes have been visited
    for (size_t i = 0; i < nodes_.size(); ++i) {
        if (!visitedNodes[i]) {
            _BFS(i, addresses, visitedNodes);
        }
    }

    return addresses;
}

vector<string> Graph::mostLikelyPath(const string& senderAddress, const string& recipientAddress) const {
    if (!addressExists(senderAddress) || !addressExists(recipientAddress))
        return vector<string>();

    size_t senderIdx = node_dict_.at(senderAddress);
    size_t recipientIdx = node_dict_.at(recipientAddress);

    vector<const Node*> predecessors = _dijkstraSSSP(senderIdx);

    vector<string> addressesPath;
    size_t currentPredecessorIdx = recipientIdx;

    // Follow path from recipient to source.
    while (predecessors[currentPredecessorIdx]) {
        const string& currentPredecessorAddress = predecessors[currentPredecessorIdx] -> address;
        addressesPath.insert(addressesPath.begin(), currentPredecessorAddress);
        currentPredecessorIdx = predecessors[currentPredecessorIdx] -> index;
    }

    // If path exists then add recipient address to the end of the path.
    if (!addressesPath.empty())
        addressesPath.push_back(recipientAddress);

    return addressesPath;
}

vector<pair<string, unsigned int>> Graph::getAddressesByCentrality(size_t samplingSize) const {
    vector<int> scores = _getBetweenessCentrality(samplingSize);
    vector<pair<string, unsigned int>> scoresWithAddress;
    
    // Build scores with node.
    for (size_t nodeIdx = 0; nodeIdx < scores.size(); ++nodeIdx) {
        if (scores[nodeIdx] >= 0) {
            const Node& node = nodes_[nodeIdx];
            scoresWithAddress.emplace_back(node.address, scores[nodeIdx]);
        }
    }

    // Comparator for sorting nodes by centrality score.
    auto comparator = [&] (const pair<string, unsigned int>& lhs, const pair<string, unsigned int>& rhs) {
        return lhs.second > rhs.second;
    };

    sort(scoresWithAddress.begin(), scoresWithAddress.end(), comparator);
    
    return scoresWithAddress;
}

long long Graph::getAddressBalance(const string& address) const {
    if (!addressExists(address))
        return 0;
    
    const Node& node = nodes_[node_dict_.at(address)];
    long long balance = 0;
    
    // Sum transaction amount of all inbound edges
    for (size_t edgeIdx : node.inbound) {
        balance += _getEdgeTransactionAmount(edgeIdx);
    }

    // Subtract transaction amount of all outbound edges
    for (size_t edgeIdx : node.outbound) {
        balance -= _getEdgeTransactionAmount(edgeIdx);
    }

    return balance;
}

size_t Graph::getAddressCount() const {
    return nodes_.size();
}

size_t Graph::getTransactionCount() const {
    return edges_.size();
}

/************************ Private Helper Methods ************************/

void Graph::_BFS(size_t startNodeIdx, vector<string>& addresses, vector<bool>& visitedNodes) const {
    queue<const Node*> q;

    visitedNodes[startNodeIdx] = true;
    q.push(&nodes_[startNodeIdx]);

    // Push and pop nodes in BFS order through queue 
    // until all nodes connected to startNode have been visited.
    while (!q.empty()) {
        const Node* currentNode = q.front();
        q.pop();

        addresses.push_back(currentNode -> address);

        for (size_t edgeIdx : currentNode -> outbound) {
            const Edge& edge = edges_[edgeIdx];
            size_t recipientIdx = edge.recipientIdx;

            if (!visitedNodes[recipientIdx]) {
                visitedNodes[recipientIdx] = true;
                q.push(&nodes_[recipientIdx]);
            }
        }
    }
}

vector<const Graph::Node*> Graph::_dijkstraSSSP(size_t sourceNodeIdx) const {

    // Holds distance from nodeIdx to source node.
    vector<double> distance(nodes_.size(), numeric_limits<double>::max());

    // Holds predecessor from nodeIdx in path to get to source node.
    vector<const Node*> predecessor(nodes_.size());

    // Comparator to get closest node in distance from priority queue.
    auto comparator = [&] (size_t lhsIdx, size_t rhsIdx) {
        return distance[lhsIdx] > distance[rhsIdx];
    };

    priority_queue<size_t, vector<size_t>, decltype(comparator)> q(comparator);
    
    distance[sourceNodeIdx] = 0;
    q.push(sourceNodeIdx);
    
    // Loop until every node reachable from source has been assigned 
    // a predecessor node in the shortest path to the source node.
    while (!q.empty()) {
        size_t currentNodeIdx = q.top();
        const Node& currentNode = nodes_[currentNodeIdx];
        q.pop();

        for (size_t edgeIdx : currentNode.outbound) {
            const Edge& edge = edges_[edgeIdx];
            size_t recipientIdx = edge.recipientIdx;

            if (distance[currentNodeIdx] + edge.weight < distance[recipientIdx]) {
                distance[recipientIdx] = distance[currentNodeIdx] + edge.weight;
                predecessor[recipientIdx] = &currentNode;
                q.push(recipientIdx);
            }
        }
    }

    return predecessor;
}  

vector<int> Graph::_getBetweenessCentrality(size_t samplingSize) const {
    if (samplingSize == 0 || samplingSize > nodes_.size())
        samplingSize = nodes_.size();

    // Centrality score of all nodes with a default score of -1 
    // to indicate nodes not visited.
    vector<int> score(nodes_.size(), -1);

    // Assign node indexes to vector to use for sampling.
    vector<size_t> nodeIdxs;
    for (size_t i = 0; i < nodes_.size(); ++i)
        nodeIdxs.push_back(i);
    
    random_shuffle(nodeIdxs.begin(), nodeIdxs.end());

    for (size_t u = 0; u < samplingSize; ++u) {
        size_t sourceIdx = nodeIdxs[u];

        // Indicate node has been visited by offsetting default -1 score.
        ++score[sourceIdx];
        
        // Get all the paths through predecessors list 
        // of all nodes from source node.
        vector<const Node*> predecessors = _dijkstraSSSP(sourceIdx);
        
        for (size_t recipientIdx = 0; recipientIdx < predecessors.size(); ++recipientIdx) {
            const Node*& predecessor = predecessors[recipientIdx];
            if (predecessor) {
                size_t currentPredecessorIdx = predecessor -> index;

                while (currentPredecessorIdx != sourceIdx) {
                    // Predecessor is detected as node in between path of
                    // source and recipient, so centrality is increased.
                    ++score[currentPredecessorIdx];
                    currentPredecessorIdx = predecessors[currentPredecessorIdx] -> index;
                }
            }
        }
    }
  
    return score;
}

long long Graph::_getEdgeTransactionAmount(size_t edgeIdx) const {
    const Edge& edge = edges_[edgeIdx];

    // Edge weight is stored as 1/(transaction amount)
    // convert back to original transaction amount.
    return llround(1.0 / edge.weight);
}

size_t Graph::_addNode(const string& address) {
    if (addressExists(address))
        return node_dict_.at(address);

    auto insertedElement = node_dict_.emplace(address, nodes_.size());
    const string& elementAddress = insertedElement.first -> first;
    size_t nodeIdx = insertedElement.first -> second;

    nodes_.emplace_back(elementAddress, nodeIdx);

    return nodeIdx;
}

size_t Graph::_addEdge(size_t senderIdx, long long amount, size_t recipientIdx) {
    vector<size_t>& senderOutbounds = nodes_[senderIdx].outbound;
    vector<size_t>& recipientInbounds = nodes_[recipientIdx].inbound;

    // Check for recipient in sender's outbound transactions 
    // to check if an edge already exists since multiple edges 
    // cannot be between sender and recipient.
    for (size_t edgeIdx : senderOutbounds) {
        const Edge& edge = edges_[edgeIdx];
        if (edge.recipientIdx == recipientIdx) {
            cerr << "WARNING: Multiple edges attempted to be added from "
                 << nodes_[senderIdx].address << " to " 
                 << nodes_[recipientIdx].address << "."
                 << "Ignoring transaction with amount " << amount << "." 
                 << endl;
            return edgeIdx;
        }
    }

    size_t edgeIdx = edges_.size();
    edges_.emplace_back(senderIdx, amount, recipientIdx);

    senderOutbounds.emplace_back(edgeIdx);
    recipientInbounds.emplace_back(edgeIdx);

    // Current edge index.
    return edgeIdx;
}

vector<string> Graph::_splitString(const string& str, char delimiter) const {
    vector<string> splits;
    stringstream ss(str);
    string curSplit;

    while (getline(ss, curSplit, delimiter)) {
        // Removes special characters.
        stringstream ssFormat(curSplit);
        ssFormat >> curSplit;

        splits.push_back(curSplit);
    }

    return splits;
}