#include <fstream>
using std::ofstream;
using std::getline;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <string>
using std::string;
using std::to_string;

#include <utility>
using std::pair;

#include <dirent.h>

#include "graph.h"

/**
 * @brief Write strings in vector to file.
 * 
 * @param v        List of strings to write into file.
 * @param filename Filename to input strings into.
 */
void writeVectorToFile(const vector<string>& v, const string& filename) {
  ofstream file;
  file.open(filename);
  for (const string& str : v) {
    file << str << "\n";
  }
  file.close();
}

/**
 * @brief Demonstration of Breadth First Search (BFS) traversal over 
 * graph of Bitcoin transactions.
 * 
 * @param g Graph.
 */
void BFSDemo(const Graph& g, const string& outputDir) {
  cout << "BFS Demo \n" << endl;

  string filename = outputDir + "/BFSDemo.txt";

  cout << "Traversing graph in BFS order..." << endl;
  writeVectorToFile(g.BFS(), filename);
  cout << "Traversal outputted to " << filename << "\n" << endl;
}

/**
 * @brief Demonstration of Dijkstra's shortest path algorithm 
 * on graph of Bitcoin transactions.
 * 
 * @param g Graph.
 * 
 * Estimates the path that a satoshi is 'most likely' to take to travel from 
 * a given sender to a given recipient. Please refer to the readme for more details.
 * 
 * Examples:
 *  Sender: 16AmxnpnfQi58JozFv69PXkfCqfcZQKesR
 *  Recipient: 3BhZfzdFCzX3p1DkQosFpgV8LKCDcPt2WX
 * 
 *  Sender: bc1q4c8n5t00jmj8temxdgcc3t32nkg2wjwz24lywv
 *  Recipient: 3J84s4rRpyG87wSKpAcxBtXFJbLwZ1B483
 * 
 *  Sender: 1PQA36EPu9YM4toTihagJuskmJ1WgPuyJ2
 *  Recipient: 16w7NznT6V7qSb22ZsadTHgJ7xxJVRGxRj
 */
void mostLikelyPathDemo(const Graph& g, const string& outputDir) {
  cout << "Shortest Path Demo \n" << endl;

  string filename = outputDir + "/pathDemo.txt";
  string senderAddress, recipientAddress;

  cout << "Input two addresses from /" + outputDir + "/transactions.csv" << endl;
  
  // Get sender address
  cout << "Sender Bitcoin Address: ";
  getline(cin, senderAddress);

  // Get recipient address
  cout << "Recipient Bitcoin Address: "; 
  getline(cin, recipientAddress);

  cout << "Getting shortest path with most bitcoin transacted..." << endl;
  vector<string> path = g.mostLikelyPath(senderAddress, recipientAddress);
  writeVectorToFile(path, filename);
  cout << "Path outputted to " << filename << "\n" << endl;
  cout << "Gettings stats on path..." << endl;

  long long totalTransacted = 0;

  for (size_t i = 1; i < path.size(); ++i) {
    totalTransacted += g.getTransactionAmount(path[i - 1], path[i]);
  }

  cout << "The path contains " << path.size() 
       << " addresses with a total of " 
       << g.convertSatoshiToBTC(totalTransacted)
       << " BTC transacted in the path from "
       << senderAddress << " to " << recipientAddress << "\n" << endl;
}

/**
 * @brief Demonstration of betweeness centrality algorithm on 
 * graph of Bitcoin transactions.
 * 
 * @param g Graph.
 * 
 * Centrality is calculated using shortest path, dijkstra's, algorithm 
 * in the graph over every node by the amount of paths each 
 * node is involved in.
 * 
 * Algorithm is expensive so sampling is 
 * sometimes required for large datasets.
 */
void centralityDemo(const Graph& g, const string& outputDir) {
  cout << "Centrality Demo \n" << endl;

  string filename;
  size_t sampleSize;

  cout << "Enter sampling size for calculating centrality " 
       << "(enter 0 for no sampling): ";

  cin >> sampleSize;

  filename = outputDir + "/" + to_string(sampleSize) + "sampleCentrality.txt";

  cout << "Getting addresses by centrality with sample size " 
       << sampleSize << "..." << endl;

  vector<pair<string, unsigned int>> centralities = g.getAddressesByCentrality(sampleSize);
  vector<string> addressesByCentrality;

  for (const auto& centrality : centralities)
    addressesByCentrality.push_back(centrality.first);

  writeVectorToFile(addressesByCentrality, filename);
  cout << "Addresses listed by centrality outputted to " << filename << "\n" << endl;

  cout << "Top 10 addresses with their balance and centrality score in graph:\n" << endl;
  for (size_t i = 0; i < 10 && i < centralities.size(); ++i) 
    cout << i + 1 << ". " 
         << centralities[i].first // Address
         << " (" << g.convertSatoshiToBTC(g.getAddressBalance(centralities[i].first)) << " BTC)" // Balance
         << " (" << centralities[i].second << ")" // Score
         << endl;
}

int main(int argc, const char * argv[]) {
  string inputFilepath = "data/transactions.csv";
  string outputDir = "data";
  bool BFSEnabled = false;
  bool mostLikelyPathEnabled = false;
  bool centralityEnabled = false;

  string currentArg;
  for (int i = 1; i < argc; ++i) {
    if (currentArg == "--input-filepath") {
      inputFilepath = argv[i];
    } else if (currentArg == "--output-directory") {
      outputDir = argv[i];
    }

    currentArg = argv[i];

    if (currentArg == "--BFS") {
      BFSEnabled = true;
    } else if (currentArg == "--most-likely-path") {
      mostLikelyPathEnabled = true;
    } else if (currentArg == "--betweenness-centrality") {
      centralityEnabled = true;
    }
  }

  // check if output directory exists
  DIR* tempDir = opendir(outputDir.c_str());
  if (tempDir) {
    closedir(tempDir);
  } else {
    cout << "Directory \"" + outputDir + "\" does not exist!" << endl;
    return 1;
  }

  // Initialize graph
  cout << "Loading /" + inputFilepath << endl;
  try {
    Graph g("./" + inputFilepath);

    cout << "Loaded " << g.getAddressCount() << " Addresses and " 
                      << g.getTransactionCount() << " Transactions \n\n" << endl;

    if (BFSEnabled) {
      BFSDemo(g, outputDir);
    }
    if (mostLikelyPathEnabled) {
      mostLikelyPathDemo(g, outputDir);
    }
    if (centralityEnabled) {
      centralityDemo(g, outputDir);
    }
  } catch (string e) {
    cout << e << endl;
    return 1;
  }

  return 0;
}