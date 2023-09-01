#!/bin/sh
find /ALCOM_CTI/log -mtime +7 -name aribo_cti_\* -exec rm {} \;

