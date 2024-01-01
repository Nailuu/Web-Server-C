#!/bin/bash

for i in {1..50}
do
	ruby client.rb &
done
wait
