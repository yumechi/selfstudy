#!/bin/sh
set -ue
# get latest submodules origin mastar
echo 'Start Update Submodules'
git submodule foreach git pull origin master
echo 'End Update Submodules'
