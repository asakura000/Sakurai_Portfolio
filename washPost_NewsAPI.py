# From NewsAPI qdatabase, collects all articles from Washington Post's "World" section and creates a json object. 

import json
import urllib
import requests
from datetime import datetime as dt


with open("mykey.txt") as f:
    API_KEY = f.read().strip()


dates = [
    "9/6/20 - 9/6/20",
    "6/7/20 - 6/13/20",
    "3/1/20 - 3/7/20",
    "12/1/19 - 12/7/19",
    "9/1/19 - 9/7/19",
    "6/2/19 - 6/8/19",
    "3/3/19 - 3/9/19"
]


all_articles = []
for d in dates:
    start, end = d.split(" - ")

    # start and end dates
    start = dt.strftime(dt.strptime(start, "%m/%d/%y"), "%Y-%m-%d")
    end = dt.strftime(dt.strptime(end, "%m/%d/%y"), "%Y-%m-%d")

    page = 1
    pageSize = 100
    num_articles = float("inf")
    while (page - 1) * pageSize < num_articles:
        parameters = {
            "apiKey": API_KEY,
            "domains": "washingtonpost.com",
            "from": start,
            "to": end,
            "pageSize": pageSize,
            "page": page,
            #"q": "international"
        }

        url = (
            "http://newsapi.org/v2/everything?" +
            urllib.parse.urlencode(parameters)
        )
        print(url)

        response = requests.get(url).json()
        if response["status"] == "ok":
            all_articles += response["articles"]
        else:
            print(response)

        if num_articles == float("inf"):
            num_articles = response["totalResults"]
        page += 1

'''
Task: 

1) go through all_articles and find all of the articles that have key = 'url' and value 'https://www.washingtonpost.com/world' as the first 37 characters in the value
2) push only these articles into a new array called filtered_articles 

'''
filtered_articles = []

sub_url_we_want = "https://www.washingtonpost.com/world"

for article in all_articles:
    whole_url = article['url']
    if(whole_url[0:36] == sub_url_we_want and article['title'] and article['description'] and article['content']): 
        print(whole_url[0:36])
        filtered_articles.append(article)
        #print(article)

    
# save accumulated articles
now = dt.strftime(dt.now(), "%Y%m%d_%H%M%S")
filename = now + ".json"
with open(filename, "w") as f:
    json.dump(filtered_articles, f)

print("Saved", len(filtered_articles), "articles.")
