#!/bin/bash

for i in {1..150}
do
	ruby client.rb &
done
wait
