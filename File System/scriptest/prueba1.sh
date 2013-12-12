#!/bin/bash
for i in {1..600}; do truncate -s 0 $i; done
for i in {601..1200}; do mkdir $i; done
