#!/bin/bash

./lem-in < test_maps/pylone_400_10_10_35_35_3_no_z > testres
if  grep -q '#> Moves = 79' testres; then
	echo "OK"
else
	echo "KO"
fi

./lem-in < test_maps/pylone_400_10_10_35_35_3_no_z_rev > testres
if  grep -q '#> Moves = 79' testres; then
	echo "OK"
else
	echo "KO"
fi

./lem-in < test_maps/pylone_1000_10_10_35_35_3_z_remake > testres
if  grep -q '#> Moves = 114' testres; then
	echo "OK"
else
	echo "KO"
fi

./lem-in < test_maps/pylone_1000_10_10_35_35_3_z_remake_rev > testres
if  grep -q '#> Moves = 114' testres; then
	echo "OK"
else
	echo "KO"
fi

./lem-in < test_maps/test_100-6_3000_25_25_75_75_5_z > testres
if  grep -q '#> Moves = 133' testres; then
	echo "OK"
else
	echo "KO"
fi

./lem-in < test_maps/test_100-6_1000_20_20_80_80_5_no_z.txt > testres
if  grep -q '#> Moves = 36' testres; then
	echo "OK"
else
	echo "KO"
fi

rm testres

