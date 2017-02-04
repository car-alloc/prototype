#!/usr/bin/env bash

# extract from students 

students=$(cut -d, -f 1 < student.dat | tr -d \" )

# extract from centers

centres=$(cut -d, -f 1 < centres.dat | tr -d \")

# multigrep!

select=$(echo $students $centres | tr ' ' \| )

grep -E $select canada-postal-codes.utf-8.csv
