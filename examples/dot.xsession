#
# sample .xsession file
#

# start idesk (we need the sleep because fluxbox will overpaint the
# idesk with the rootCommand
(sleep 2; idesk &) &

# start fluxbox
exec fluxbox
exit $?

