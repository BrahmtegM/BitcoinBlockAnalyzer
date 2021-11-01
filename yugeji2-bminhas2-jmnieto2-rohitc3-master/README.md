# CS225 Final Project: Bitcoin Transactions

## Team Members: Javier Nieto, Hugo Ji, Brahmteg Minhas, Rohit Chatterjee

### Code
The graph structure and methods are implemented in graph.h and graph.cpp. The command-line interface for running the methods is implemented in main.cpp. All C++ files (except for tests) are located in the root directory.

### Data
The default input file **transactions.csv** is located in the **data** directory. The input file must be formatted as a .csv file with each line defining a transaction. Each line must contain the address of the sender, the amout of the transaction in satoshis, and the address of the recipient in that exact order and separated by commas. By default, all output files will be saved in the **data** directory.

### Commands
To build, run 'make' in the root directory. This will generate an executable called **main** which you can run using './main'. By default, running './main' will not run any of the methods on the graph since you must specify which methods you want to run using flags.

#### Flags
    --input-filepath
Specifies the path of the file to load transactions from. By default, it is **data/transactions.csv.**

    --output-directory
Specifies the path of the folder to place output files in. By default, it is **data.**

   --BFS
If included, breadth-first search will run.

    --most-likely-path
If included, the method which estimates the most likely path between two addresses will run.

    --betweenness-centrality
If included, the method which calculates betweenness centrality will run.

To use flags, type './main' followed by the flags you want to use, separated by spaces. For example:

    ./main --input-filepath data/input.csv --BFS --betweenness-centrality

### Tests
The tests are located in the **tests** directory. They test the following:
 - Whether the graph constructor loads all addresses and transactions from the input file
 - Whether each address has the correct balance (satoshis gained minus satoshis lost)
 - Whether breadth-first search works as intended
 - Whether the most likely path algorithm works as intended
 - Whether the betweenness centrality algorithm works as intended

To build the tests, run 'make test' in the root directory. Then, you can run the tests using './tests'.

### Presentation
https://drive.google.com/file/d/1r9i4VMdvgjVei1AIRRe2GXIgZLgOrhmk/view?usp=sharing