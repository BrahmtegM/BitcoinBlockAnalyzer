#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <ostream>
using std::ostream;

#include <string>
using std::string;

#include <utility>
using std::pair;

#include "../graph.h"
#include "../catch/catch.hpp"

TEST_CASE("test_test_case", "[classic]") {
  REQUIRE(1 == 1); 
}

/********************* Test Graph::addressExists() **********************/

TEST_CASE("test_tiny_addressExists", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.addressExists("1FckoRapKaVE2kktJ2T3oVoGydjQkGy2z2") == true);
  REQUIRE(g.addressExists("357oZdnMBzc4KSgeqBrS29BgFUpFmfqpsj") == true);
  REQUIRE(g.addressExists("bc1q58rk9g9lrys3m63ytyy3e9n0ruhkkpazs3hhxh") == true);
  REQUIRE(g.addressExists("12UAgcBwBcpjRkdowY4tRVwuX7GPLyaNqT") == false);
}

TEST_CASE("test_small_addressExists", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.addressExists("bc1q58rk9g9lrys3m63ytyy3e9n0ruhkkpazs3hhxh") == true);
  REQUIRE(g.addressExists("bc1qhds964u6vl9dlawdnrrr5taxvleuuk7gannmd0") == true);
  REQUIRE(g.addressExists("3KzhJT5Yckg2TGaqgqTsemqYxdtF9nGT6R") == true);
  REQUIRE(g.addressExists("12UAgcBwBcpjRkdowY4tRVwuX7GPLyaNqT") == false);
}

TEST_CASE("test_large_addressExists", "[classic]") {
  Graph g("./data/transactions.csv");
  
  REQUIRE(g.addressExists("bc1q58rk9g9lrys3m63ytyy3e9n0ruhkkpazs3hhxh") == true);
  REQUIRE(g.addressExists("bc1qqn0k6e9pxymlps026wmqt9qp28yrfdjnd33njy") == true);
  REQUIRE(g.addressExists("124zkk1zpDgNeWRBPsTbtDLWiDEPwT8wwB") == true);
  REQUIRE(g.addressExists("12UAgcBwBcpjRkdowY4tRVwuX7GPLyaNqT") == false);
}

/********************* Test Graph::getAddressCount() ***********************/

TEST_CASE("test_tiny_getAddressCount", "[classic]") {
  Graph g("./tests/tinySample.csv");

  REQUIRE(g.getAddressCount() == 9);
}

TEST_CASE("test_small_getAddressCount", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.getAddressCount() == 33);
}

TEST_CASE("test_medium_getAddressCount", "[classic]") {
  Graph g("./tests/mediumSample.csv");
  
  REQUIRE(g.getAddressCount() == 92633);
}

/********************* Test Graph::getTransactionCount() ***********************/

TEST_CASE("test_tiny_getTransactionCount", "[classic]") {
  Graph g("./tests/tinySample.csv");

  REQUIRE(g.getTransactionCount() == 5);
}

TEST_CASE("test_small_getTransactionCount", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.getTransactionCount() == 26);
}

TEST_CASE("test_medium_getTransactionCount", "[classic]") {
  Graph g("./tests/mediumSample.csv");
  
  REQUIRE(g.getTransactionCount() == 100000);
}

/***************** Test Graph::getTransactionAmount() *******************/

TEST_CASE("test_tiny_getTransactionAmount", "[classic]") {
  Graph g("./tests/smallSample.csv");

  string sender1 = "1FckoRapKaVE2kktJ2T3oVoGydjQkGy2z2";
  string recipient1 = "1CUTyyxgbKvtCdoYmceQJCZLXCde5akiX2";

  string sender2 = "bc1qxyvjetj7qdfwxctmgrtxlpa3cts3f5nt86d6kq";
  string recipient2 = "bc1qauqcnzc9nq09pjeqkwnnu7xf5xwu3kedxe83yr";

  REQUIRE(g.getTransactionAmount(sender1, recipient1) == 299985332);
  REQUIRE(g.getTransactionAmount(sender2, recipient2) == 1322231424);
}

TEST_CASE("test_small_getTransactionAmount", "[classic]") {
  Graph g("./tests/smallSample.csv");

  string sender1 = "bc1qrxfa055gxfvp6w36xcwgv8zx0fwqqsr4kxwx57";
  string recipient1 = "32Kj5YD1q4wPNxTJcJ2JkbQ3aavG1tTeT6";

  string sender2 = "3G6SnudkJPAjcruCFzFq8ng8ncvP4ykAYZ";
  string recipient2 = "3BKn5fzKRx7stkBQyGqehWdjJxYCZhQwga";

  REQUIRE(g.getTransactionAmount(sender1, recipient1) == 115995001);
  REQUIRE(g.getTransactionAmount(sender2, recipient2) == 121035297);
}

TEST_CASE("test_medium_getTransactionAmount", "[classic]") {
  Graph g("./tests/mediumSample.csv");

  string sender1 = "3G6SnudkJPAjcruCFzFq8ng8ncvP4ykAYZ";
  string recipient1 = "3BKn5fzKRx7stkBQyGqehWdjJxYCZhQwga";

  string sender2 = "bc1qrxfa055gxfvp6w36xcwgv8zx0fwqqsr4kxwx57";
  string recipient2 = "32Kj5YD1q4wPNxTJcJ2JkbQ3aavG1tTeT6";

  REQUIRE(g.getTransactionAmount(sender1, recipient1) == 121035297);
  REQUIRE(g.getTransactionAmount(sender2, recipient2) == 115995001);
}

/****************** Test Graph::getAddressBalance() *********************/

TEST_CASE("test_tiny_getAddressBalance", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.getAddressBalance("1CUTyyxgbKvtCdoYmceQJCZLXCde5akiX2") == 464283737);
  REQUIRE(g.getAddressBalance("bc1qq904ynep5mvwpjxdlyecgeupg22dm8am6cfvgq") == -2940190);
}

TEST_CASE("test_small_getAddressBalance", "[classic]") {
  Graph g("./tests/smallSample.csv");

  REQUIRE(g.getAddressBalance("35Geev9cEfdTyt35tBXwWySQgodnRwvAJu") == 1115725);
  REQUIRE(g.getAddressBalance("bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej") == 21498496567);
  REQUIRE(g.getAddressBalance("bc1qm4h0wanqh6vw3rft5xnz3dl6k5ultzds6xgaqj") == -1094);
}

TEST_CASE("test_medium_getAddressBalance", "[classic]") {
  Graph g("./tests/mediumSample.csv");

  REQUIRE(g.getAddressBalance("3G6SnudkJPAjcruCFzFq8ng8ncvP4ykAYZ") == -121035297);
  REQUIRE(g.getAddressBalance("bc1qauqcnzc9nq09pjeqkwnnu7xf5xwu3kedxe83yr") == 3303441);
  REQUIRE(g.getAddressBalance("bc1q57z43n85cwph5s9vydjx4lzf7yv8q2g50vf7gk") == -1593994);
}

/************************* Test Graph::BFS() ****************************/

TEST_CASE("test_tiny_BFS", "[classic]") {
  Graph g("./tests/tinySample.csv");
  
  vector<string> solution = {
    "1FckoRapKaVE2kktJ2T3oVoGydjQkGy2z2", "1CUTyyxgbKvtCdoYmceQJCZLXCde5akiX2",
    "357oZdnMBzc4KSgeqBrS29BgFUpFmfqpsj", "bc1qxyvjetj7qdfwxctmgrtxlpa3cts3f5nt86d6kq",
    "bc1qauqcnzc9nq09pjeqkwnnu7xf5xwu3kedxe83yr", "bc1qq904ynep5mvwpjxdlyecgeupg22dm8am6cfvgq",
    "13EV17cfgnK4MsxoQwoVqNr3hSqaGJQob2", "bc1q866sv2stppgtywjejsjj3xsfrsm5e2f5zqwsx9z0qcx9k34f0xlqaslnwr",
    "bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej"
  };

  vector<string> testTraversal = g.BFS();

  REQUIRE(testTraversal.size() == solution.size());
  REQUIRE(testTraversal == solution);
}

TEST_CASE("test_tiny2_BFS", "[classic]") {
  Graph g("./tests/tinySample2.csv");
  
  vector<string> solution = {
    "1BNAYuUfSiY6FCg5LCiJkTYkQAEaAxjUiu", "1L5QgNHtcyBFyndhVhY353HyWaXCag4Jx",
    "3CYuiCMJzQ9vgomtSE3ZF7ygNfSKqDqjSb", "38kqvN2sNxJtq4bSY9sYioyk4R7Zo3E5FP",
    "33PWEGbE5TokjHm7mAGbb3ZmcLqTWvZ6cr", "3N4t1HzpsJipALh1U2dyssvWu9TsNQk4wx",
    "3JCBEDhEE1vZyEuB5gz8hM8gtPyAfKPctX", "3DVZ9NRjKixMNY3ipTbwdPvwKeCqcuQnUV",
    "37owVJyjVYLwsTCLRmJ2zD25F5dQZM9okp", "3Hg7czYUWtMbaXe9CgcyLW2fdwqvBy4END",
    "39yzT9McTdzgA9G31uECmXAswfjnhXBtpK", "bc1qv8jxnyy5ta396ztlwptjer5d4q6cntv0qw5rka",
    "3KbdivZweu1JmpjvmRsGhXUrXPsGPEWg8W", "3NqD4hJmhK2ZL7NrQ7J3MFcjxn51kxbkiB"
  };

  vector<string> testTraversal = g.BFS();

  REQUIRE(testTraversal.size() == solution.size());
  REQUIRE(testTraversal == solution);
}

/****************** Test Graph::mostLikelyPath() ***********************/

TEST_CASE("test_tiny_mostLikelyPath", "[classic]") {
  Graph g("./tests/tinySample.csv");
  
  vector<string> solution = {
    "357oZdnMBzc4KSgeqBrS29BgFUpFmfqpsj", 
    "1CUTyyxgbKvtCdoYmceQJCZLXCde5akiX2"
  };
  vector<string> testPath = g.mostLikelyPath("357oZdnMBzc4KSgeqBrS29BgFUpFmfqpsj", "1CUTyyxgbKvtCdoYmceQJCZLXCde5akiX2");

  REQUIRE(testPath == solution); 
}

TEST_CASE("test_tiny2_mostLikelyPath", "[classic]") {
  Graph g("./tests/tinySample2.csv");
  
  vector<string> solution = {
    "3CYuiCMJzQ9vgomtSE3ZF7ygNfSKqDqjSb", 
    "38kqvN2sNxJtq4bSY9sYioyk4R7Zo3E5FP",
    "33PWEGbE5TokjHm7mAGbb3ZmcLqTWvZ6cr",
    "3N4t1HzpsJipALh1U2dyssvWu9TsNQk4wx",
    "3JCBEDhEE1vZyEuB5gz8hM8gtPyAfKPctX"
  };
  vector<string> testPath = g.mostLikelyPath("3CYuiCMJzQ9vgomtSE3ZF7ygNfSKqDqjSb", "3JCBEDhEE1vZyEuB5gz8hM8gtPyAfKPctX");
  
  REQUIRE(testPath == solution); 
}

TEST_CASE("test_small_mostLikelyPath", "[classic]") {
  Graph g("./tests/smallSample.csv");
  
  vector<string> solution = {
    "3AWuoNoTTC3mUePdgmWwLxt8vZaPuQbVZC", 
    "bc1qzjeg3h996kw24zrg69nge97fw8jc4v7v7yznftzk06j3429t52vse9tkp9",
    "bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej"
  };
  vector<string> testPath = g.mostLikelyPath("3AWuoNoTTC3mUePdgmWwLxt8vZaPuQbVZC", "bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej");
  
  REQUIRE(testPath == solution); 
}

TEST_CASE("test_small2_mostLikelyPath", "[classic]") {
  Graph g("./tests/smallSample2.csv");
  
  vector<string> solution = {
    "bc1qpxcpujxxzhly3myuuhushw23ru6h07ty3cn949z9yt40l7x9c63qgmzf7a", 
    "bc1qxpuxdu8uc06fmrfxyr3ygyq260q3u8uehplw389tz4f70gulnkjqr96hwy",
    "bc1qfwf5zkaqxntynnt930npt7wvyre9kswc9pwpu05f6pcygnrd6fuqk27tcx",
    "bc1qs905qy3p6nzsl5fzkfezxfezd2d8azm65hvavvfhngclj7u50f2q43z9x6",
    "13TESPS6z8y6LmnQNba8t84ePtjBzvyBS5",
    "1JUToCyRL5UwgeucjnFAagKs4v1YqhjT1d"
  };
  vector<string> testPath = g.mostLikelyPath("bc1qpxcpujxxzhly3myuuhushw23ru6h07ty3cn949z9yt40l7x9c63qgmzf7a", "1JUToCyRL5UwgeucjnFAagKs4v1YqhjT1d");
  
  REQUIRE(testPath == solution); 
}

TEST_CASE("test_medium_mostLikelyPath", "[classic]") {
  Graph g("./tests/mediumSample.csv");
  
  vector<string> solution = {};
  vector<string> testPath = g.mostLikelyPath("bc1qhlc5vdm6sy6t4zvl865mqpeal76tl8eu0nzyku", "3MjEy16rTVjJH9MbrHXuL3GbjUxSXzqxzF");
  
  REQUIRE(testPath == solution); 
}

/************** Test Graph::getAddressesByCentrality() ******************/

TEST_CASE("test_tiny_getAddressesByCentrality_sampling", "[classic]") {
  Graph g("./tests/tinySample.csv");
  
  vector<pair<string, unsigned int>> testCentralityScores = g.getAddressesByCentrality(2);

  REQUIRE(testCentralityScores.size() == 2); 
}

TEST_CASE("test_tiny2_getAddressesByCentrality", "[classic]") {
  Graph g("./tests/tinySample2.csv");
  
  vector<pair<string, unsigned int>> testCentralityScores = g.getAddressesByCentrality();
  vector<string> testAddressesByCentrality;

  REQUIRE(testCentralityScores.front().second == 4);
  REQUIRE(testCentralityScores.back().second == 0);

  for (const auto& centrality : testCentralityScores)
    testAddressesByCentrality.push_back(centrality.first);

  vector<string> solution = {
    "bc1qv8jxnyy5ta396ztlwptjer5d4q6cntv0qw5rka", "33PWEGbE5TokjHm7mAGbb3ZmcLqTWvZ6cr", 
    "3KbdivZweu1JmpjvmRsGhXUrXPsGPEWg8W", "39yzT9McTdzgA9G31uECmXAswfjnhXBtpK", 
    "38kqvN2sNxJtq4bSY9sYioyk4R7Zo3E5FP", "3N4t1HzpsJipALh1U2dyssvWu9TsNQk4wx", 
    "3DVZ9NRjKixMNY3ipTbwdPvwKeCqcuQnUV", "37owVJyjVYLwsTCLRmJ2zD25F5dQZM9okp", 
    "3Hg7czYUWtMbaXe9CgcyLW2fdwqvBy4END", "3CYuiCMJzQ9vgomtSE3ZF7ygNfSKqDqjSb", 
    "1L5QgNHtcyBFyndhVhY353HyWaXCag4Jx", "3JCBEDhEE1vZyEuB5gz8hM8gtPyAfKPctX", 
    "1BNAYuUfSiY6FCg5LCiJkTYkQAEaAxjUiu", "3NqD4hJmhK2ZL7NrQ7J3MFcjxn51kxbkiB" 
  };

  REQUIRE(testAddressesByCentrality.size() == solution.size());
  REQUIRE(testAddressesByCentrality == solution); 
}

TEST_CASE("test_small2_getAddressesByCentrality", "[classic]") {
  Graph g("./tests/smallSample2.csv");
  
  vector<pair<string, unsigned int>> testCentralityScores = g.getAddressesByCentrality();
  vector<string> testAddressesByCentrality;

  REQUIRE(testCentralityScores.front().second == 44);
  REQUIRE(testCentralityScores.back().second == 0);

  vector<string> solution = {};
  
  REQUIRE(testAddressesByCentrality.size() == solution.size());
}

TEST_CASE("test_small2_getAddressesByCentrality2", "[classic]") {
  Graph g("./tests/smallSample2.csv");
  
  vector<pair<string, unsigned int>> testCentralityScores = g.getAddressesByCentrality();
  
  for (pair<string, unsigned int> p : testCentralityScores) {
    if (p.first ==  "1GJKqGx79ec2anGhXNXB3vFsKHoRWRaSwa")
        REQUIRE(p.second == 4);
    
    if (p.first == "17A16QmavnUfCW11DAApiJxp7ARnxN5pGX")
        REQUIRE(p.second == 0);
    
    if (p.first == "1DW3BBZVE9K1ntWiJJ4qC4wJDNMsYs8Lva")
        REQUIRE(p.second == 12);
    
    if (p.first == "bc1q24z5c8nyxc03ux584803ndqk5vp286x0fydn3y37e276etmheugqra9k4c")
        REQUIRE(p.second == 0);
    
    if (p.first == "13TESPS6z8y6LmnQNba8t84ePtjBzvyBS5")
        REQUIRE(p.second == 10);
  }
}

TEST_CASE("test_small3_getAddressesByCentrality", "[classic]") {
  Graph g("./tests/smallSample3.csv");

  vector<pair<string, unsigned int>> scores = g.getAddressesByCentrality();

  for (pair<string, unsigned int> p : scores) {
    if (p.first == "3J84s4rRpyG87wSKpAcxBtXFJbLwZ1B483")
      REQUIRE(p.second == 3);
    
    if (p.first == "bc1qfwf5zkaqxntynnt930npt7wvyre9kswc9pwpu05f6pcygnrd6fuqk27tcx")
      REQUIRE(p.second == 0);
  }
}

TEST_CASE("test_small_getAddressesByCentrality", "[classic]") {
  Graph g("./tests/smallSample.csv");

  vector<pair<string, unsigned int>> scores = g.getAddressesByCentrality();

  for (pair<string, unsigned int> p : scores) {
    if (p.first == "bc1q0a6drfy4d2rny0hxuvqkcql58aav56tfz8sagc")
      REQUIRE(p.second == 4);
    
    else if (p.first == "bc1qwqdg6squsna38e46795at95yu9atm8azzmyvckulcc7kytlcckxswvvzej")
      REQUIRE(p.second == 0);
    
    else if (p.first == "3BKn5fzKRx7stkBQyGqehWdjJxYCZhQwga")
      REQUIRE(p.second == 5);
  }
}