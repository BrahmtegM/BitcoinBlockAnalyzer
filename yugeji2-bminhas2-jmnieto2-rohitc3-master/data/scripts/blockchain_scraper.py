from datetime import datetime, timedelta
import requests
import csv

# Fields for csv dump
FIELDS = [
    "sender",
    "value",
    "receiver"
]


def get_blocks_since(timestamp):
    """Get basic info of blocks since timestamp from blockchain.info API

    Params
    ------
    timestamp : int
      Unix time in milliseconds

    Return
    ------
    list
      Block info in the following format:
      [
        {
            "height" : 166107,
            "hash" : "00000000000003823fa3667d833a354a437bdecf725f1358b17f949c991bfe0a",
            "time" : 1328830483
        },
        ...
      ]
    """
    return requests.get(f"https://blockchain.info/blocks/{int(timestamp)}?format=json").json()["blocks"]


def get_block_transactions(block_height):
    """Get block transactions from blockchain.info API

    Params
    ------
    block_height : int
      Block height to get transactions from

    Return
    ------
    list
      list of transactions in the following format:
      [
        {
          "hash":"b6f6991d03df0e2e04dafffcd6bc418aac66049e2cd74b80f14ac86db1e3f0da",
          "ver":1,
          "vin_sz":1,
          "vout_sz":2,
          "lock_time":"Unavailable",
          "size":258,
          "relayed_by":"64.179.201.80",
          "block_height, 12200,
          "tx_index":"12563028",
          "inputs":[
            {
              "prev_out":{
                "hash":"a3e2bcc9a5f776112497a32b05f4b9e5b2405ed9",
                "value":"100000000",
                "tx_index":"12554260",
                "n":"2"
              },
              "script":"76a914641ad5051edd97029a003fe9efb29359fcee409d88ac"
            }
          ],
          "out":[
            {
              "value":"98000000",
              "hash":"29d6a3540acfa0a950bef2bfdc75cd51c24390fd",
              "script":"76a914641ad5051edd97029a003fe9efb29359fcee409d88ac"
            },
            {
                "value":"2000000",
                "hash":"17b5038a413f5c5ee288caa64cfab35a0c01914e",
                "script":"76a914641ad5051edd97029a003fe9efb29359fcee409d88ac"
            }
          ]
      }
      ...
    ]     
    """
    return requests.get(f"https://blockchain.info/rawblock/{int(block_height)}").json()["tx"]


if __name__ == "__main__":
    # Number of days since today to get transactions
    days = int(input("Enter the number of days since today to collect transactions: "))

    # Date to scrape blocks from to today in milliseconds
    blocks_since = datetime.today() - timedelta(days=days)

    print(f"Obtaining blocks since {blocks_since}")

    blocks = get_blocks_since(blocks_since.timestamp()*1000)

    writer = csv.DictWriter(open("data/transactions.csv", "w+", newline='', encoding='utf-8'), fieldnames=FIELDS)
    transactions = {}

    print(f"Processing a total of {len(blocks)} blocks...")

    for block in blocks:
        block_height = block["height"]

        print(f"Processing block {block_height}...")

        for tx in get_block_transactions(block_height):
            # Only track simple transactions with either one input or one output
            if len(tx["inputs"]) == 1:
                input = tx["inputs"][0]["prev_out"]
                # Ignore coinbase where input does not have a previous output
                if input:
                    for output in tx["out"]:
                        # Ignore change sent back to sender
                        if "addr" in output and output["addr"] != input["addr"]:
                            sender_addr = input["addr"]
                            receiver_addr = output["addr"]
                            id = f"{sender_addr}_{receiver_addr}"
                            if id in transactions:
                                transactions[id] += output["value"]
                            else:
                                transactions[id] = output["value"]
            elif len(tx["out"]) == 1:
                output = tx["out"][0]
                for input in tx["inputs"]:
                    input = input["prev_out"]
                    if output["addr"] != input["addr"]:
                        sender_addr = input["addr"]
                        receiver_addr = output["addr"]
                        id = f"{sender_addr}_{receiver_addr}"
                        if id in transactions:
                            transactions[id] += input["value"]
                        else:
                            transactions[id] = input["value"]

    for id, tx_amt in transactions.items():
        addrs = id.split("_")
        writer.writerow({
            "sender": addrs[0],
            "value": tx_amt,
            "receiver": addrs[1]
        })
