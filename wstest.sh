#!/bin/bash

urls=(
	"localhost:1234"
	"localhost:1235"

	"localhost:1236"
	"--resolve sameport1:1236:127.0.0.1 sameport1:1236"
	"--resolve sameport2:1236:127.0.0.1 sameport2:1236"
	
	"localhost:1237/doesNotExist"
	"-X POST --data-binary dataDefinitelyBiggerThan10Bytes localhost:1237"
	
	"localhost:1238"
	"localhost:1238/someLocation/"

	"localhost:1239/"
	"-X POST --data-binary somedata localhost:1239/file.txt"
	"-X DELETE localhost:1239"

	"localhost:1239/dumpster"
	"-X POST --data-binary someData localhost:1239/dumpster/file.txt"
	"-X DELETE localhost:1239/dumpster/file.txt"
	"-X LOLOLOLOLOL localhost:1239"
)

for url in "${urls[@]}"; do
	echo "Requesting: $url"
	curl $url
	echo -e "\n"
done
