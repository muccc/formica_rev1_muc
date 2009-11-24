#!/bin/bash

VER=$1

git archive -o formica-$VER.tar --prefix=formica-$VER/ $VER
bzip2 formica-$VER.tar
