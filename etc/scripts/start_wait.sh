#!/bin/sh

############################################################
# wait to goahead start					   #
############################################################

start_wait() {
    # wait to web full load

    while [ ! -f /var/run/goahead.str ]; do
        sleep 1
    done
}
