set pagination off
set confirm off
delete breakpoints
break 38 if count >= 28 && count <= 35
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
run -100 100 3 > /dev/null
quit
