#!/bin/bash
for i in {1..2000}
do
   ps aux | grep "dark"
   echo "---------------------------------------"
done
