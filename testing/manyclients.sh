#!/bin/bash

for i in {1..5000}
do
  ruby client.rb &
done
wait
