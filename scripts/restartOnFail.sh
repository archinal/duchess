#!/usr/bin/env bash

# Given a program to run via $1, runs that program and restarts it when it crashes

RC=1
while [ $RC -ne 0 ]; do
   `$1`
   RC=$?
done