#!/usr/bin/env python
import json
from hashlib import sha256

common_passwords=["000000", "1111", "111111", "11111111", "112233", "121212", "123123", "123321", "1234", "12345", "123456",
                  "1234567", "12345678", "123456789", "1234567890", "123qwe", "131313", "159753", "1leila1", "1qaz2wsx",
                  "2000", "555555", "654321", "666666", "6969", "696969", "777777", "7777777", "987654321", "aaaaaa", "abc123",
                  "access", "admin", "amanda", "andrew", "arschloch", "asdfgh", "ashley", "asshole", "austin", "baseball",
                  "basteln", "batman", "berlin", "biteme", "buster", "charlie", "cheese", "chelsea", "computer", "dallas",
                  "daniel", "dragon", "ficken", "football", "freedom", "fuck", "fuckme", "fuckyou", "george", "ginger",
                  "hallo", "hallo1", "hallo123", "harley", "hockey", "hunter", "iloveyou", "jennifer", "jessica", "jordan",
                  "joshua", "killer", "klaster", "lebron", "letmein", "love", "maggie", "master", "matrix", "matthew",
                  "michael", "michelle", "monkey", "mustang", "nicole", "niklas23", "pass", "password", "password1",
                  "passwort", "passwort1", "pepper", "princess", "pussy", "qazwsx", "qwerty", "qwertyuiop", "qwertz",
                  "ranger", "robert", "schalke04", "schatz", "shadow", "soccer", "starwars", "summer", "sunshine",
                  "superman", "taylor", "thomas", "thunder", "tigger", "trustno1", "yankees", "zxcvbn", "zxcvbnm"]


data={}
for password in common_passwords:
    digest=sha256(bytes(password,"utf-8")).hexdigest()
    #print (password,digest)
    data[digest]=password

#print(json.dumps(data,indent=4,sort_keys=True))


