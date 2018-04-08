if  [ $1 = "1" ]; then
    bin/DifferencePass resources/example1.ll c d
elif [ $1 = "2" ]; then
    bin/DifferencePass resources/example2.ll x y
else
    echo "Please enter the option 1 or 2 for the example file you would like to run difference analysis on."
fi
