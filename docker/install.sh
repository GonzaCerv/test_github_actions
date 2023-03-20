#!/usr/bin/env bash

apt-get update && apt-get install --no-install-recommends -y $(cat requirements.txt)
pip3 install -r requirements_pip.txt
