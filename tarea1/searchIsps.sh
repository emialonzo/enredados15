#!/bin/sh

search=""
echo "Googling: $@"
for term in $@; do
  search="$search%20$term"
done
curl -I "http://www.google.com/search?q=$search"