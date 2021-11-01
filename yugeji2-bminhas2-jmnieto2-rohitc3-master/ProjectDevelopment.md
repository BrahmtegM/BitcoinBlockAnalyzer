## 4/17
We started the meeting by discussing the implementation of the makefile. None of us have much experience making a makefile so we looked to the example makefile as a model. 

We then started discussing the possible implementation of the graph, going over the pros and cons of using a linked list versus an array for the list of edges on a node. 
Looking over the algorithms we planned to implement (Dijikstra’s and Betweenness centrality), we are leaning towards using an array as our data collection implementation 
cleans the data somewhat, making us less likely to need to delete a node which linked lists are more efficient at. The ability to access certain elements in O(1) time might 
also be beneficial when implementing our algorithms and traversal. 

We then divided the work between the four of us where one converts the data from online to a csv file through a python script. One converts the csv file to a struct containing 
sender address, transaction amount and receiver address. One sets up the base files and implementation for the node and edge class and one creates the test files. 

## 4/25
During the week, we were able to get a basic file reader function for reading our Bitcoin transactions inside a csv data file to extract the sender as a string, the receiver as a string, and the transaction amount as a long long.

We had some discussion on how were specifically implementing an adjacency list for our graph and decided on having an `unordered_map<string, list>` mapping Bitcoin addresses to a list of their receivers(Bitcoin addresses/nodes on the graph which the address has sent Bitcoin to) where the receivers have a pointer to their respective transactions, or edges, on the graph. Some basic test cases were also written for the graph class methods we plan on implementing in the next week.

## 5/3 
We worked on the structure of the graph class this past week, structuring the outline of nodes and their transactions so that Dijkstra's algorithm and concurrently the betweenness centrality can be implemented as simply and efficiently as possible. We had a discussion about the efficiency of betweenness centrality and a possible O(n) runtime leading to a debate about the usefulness of memoization. To improve the runtime, we brought up the possibility of sampling which would decrease the time and hardware requirements of the algorithm while theoretically decreasing the accuracy slightly. Because betweenness centrality looks for the most connected node, not going through every node wouldn't lead to as big of an accuracy decrease as the most central node would have a lot of connections and thus be more likely to be close to a randomly selected node. Therefore, we agreed to explore sampling moving forward in our implementation of betweenness centrality. 

We also talked about some design choices, such as what data type to use for transactions discussing the possibility of using a long long along with others but we shelved the discussion until we get slightly further in our algorithm implementation. 

Planning ahead, we set roles for the rest of the week, delegating responsibility for test cases and the implementation of the traversal, Dijkstra's and betweenness centrality. We hope to finish these before the end of the weekend, giving us ample time to prepare and record the presentation. 

## 5/9
We added certain functions and cleaned up code on the Graph.cpp and Graph.h to make it easier to read and understand our code. Specifically, we added an edges vector to more accurately define the distinctions between nodes and edges and make functions based around them easier. This leads to our graph acting more as an adjacency list and saves us some memory space. More importantly, it leads to us using less pointers making the code simpler and easier to manage. 

Our Breadth First Search algorithm was tested and ran as expected within our test cases.

We also implemented the betweenness centrality algorithm, creating a function for a single node's centrality. We then used this function to calculate the most central nodes. However, the runtime of this algorithm still seems long to us, with a runtime of O(n^3), so we plan to explore how to lower this runtime in the next few days. During the meeting, we all collaborating on a change to our algorithm so that it outputs a ranking of the most "central" to least "central" nodes, allowing our outputs to more closely allign with what we set forth to do in our Project Proposal.  

In the next few days, we intend to implement Dijkstra's fully, and finish our test cases, which should finish the code portion of the final project. We also need to write our results document and prepare and record our video. 
