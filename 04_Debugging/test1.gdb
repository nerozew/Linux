set pagination off
set confirm off
delete breakpoints
break 38 if current % 5 == 0
commands
silent
echo @@@\n
print start
print stop
print step
print current
print count
continue
end
run 1 12 > /dev/null
quit
