#!/bin/bash

printf "main\n"
./otp
./otp 256 #no '-a' flag passed
./otp -a 256 #no key file passed
./otp -a 
./otp -a key 256
printf "\n"

printf "key\n" 
./otp -a key 
./otp -a key 256
printf "\n"

printf "encode\n"
./otp -a encode key.txt
./otp encode key.txt
printf "\n"

printf "decode\n" 
./otp -a decode key.txt

# Fruits=('Apple' 'Banana' 'Orange')
# Fruits[0]="Apple"
# Fruits[1]="Banana"
# Fruits[2]="Orange"

# printf "first %s\n" Fruit[0]

#stub print something to show working or not working 

