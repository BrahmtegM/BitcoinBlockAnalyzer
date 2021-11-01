# CS 225 Data Structures
## Project Proposal (yugeji2-bminhas2-jmnieto2-rohitc3)

1. **Leading Question** Using the Bitcoin blockchain network, we would like to analyze transactions to find the most probable path between two addresses to highlight how connected those addresses are. Then, we would like to find the most important, or central, addresses on the network that control most of the flow of bitcoin and search through the addresses's transactions to calculate exactly how much Bitcoin was transferred. 

2. **Dataset Acquisition and Processing** We propose using the Bitcoin transactions from the Bitcoin blockchain. We will just focus on the total amount of bitcoin sent between two addresses in a small span of time since the blockchain may be too large for us to manage. We utilized [blockchain.com's api](https://www.blockchain.com/api/blockchain_api) to collect a day of transactions (approximately 900,000 transactions). Then, we combined all transactions going the same direction between addresses into one edge representing the total amount of Bitcoin sent from one address to the other to maintian a simple graph structure. Also, any transactions where the sender sends bitcoin to themselves are ignored as those transactions do not provided any useful insight into our analysis. We will store edge weights as (1/total amount of bitcoin transacted). We only collected simple transactions with one input or one ouput into a csv with sender address, total transaction amount, and receiver address through a simple python script.

3. **Graph Algorithms**  For our graph traversal, we propose to implement a breadth first search using an iterator to iterate through all stored Bitcoin addresses (the weights of the edges are disregarded for the traversal algorithm). The input to the BFS will be every untraversed node with the first input being the sender in the first transaction in the block. Traversing through an address's transactions allows for calculating the amount of Bitcoin that has been transferred through the address, and if the whole Bitcoin blockchain were analyzed, the balance for every address could be calculated. 

For one of our algorithms, we propose implementing [Dijikstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) to calculate the shortest path between any two addresses. The input will be the sender Bitcoin address and the receiver Bitcoin address, and the output will be the shortest path from the sender to the receiver with the most Bitcoin transacted. This will allow us to see how bitcoin(s) exchange hands and detect if any two addresses are closely related. 

For our final algorithm, we propose utilizing our Dijikstra's Algorithm to implement the [Betweenness Centrality Algorithm](https://en.wikipedia.org/wiki/Betweenness_centrality) to find addresses that are the most connected on the Bitcoin network and transacted the most bitcoin. This would use both the edge weights and amount of connections for each address.  

4. **Timeline** 

   April 9 - complete project proposal and team contract
   
   April 12 - Finalize Proposal and collect transactions from blockchain
    
   April 16 - Setup Makefile
  
   April 18 - Convert array to graph and write BFS iterator and tests
   
   April 23 - finish writing Dijikstra’s algorithm and tests
   
   April 30 - finish writing Betweenness Centrality algorithm and tests
	 
   May 7 - finish report
	 
   May 8 - record presentation
	 
   May 11 - finish final touches on presentation and submit
