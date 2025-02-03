#!/usr/bin/env python
"""Brute force based password cracker"""
from hashlib import sha256

# globale variablen (böse)
allowed_chars="abcdefghijklmnopqrstuvwxyz"
num_chars=len(allowed_chars)
minlength=1
maxlength=5
searched_password=None
found_password=None

def compare(password_try):
    global found_password
    encrypted_try=sha256(bytes(password_try,"utf-8")).hexdigest()
    if searched_password == encrypted_try:
        found_password=password_try
        return True
    return False


def iterate_over_position(position, pw_buffer):
    for character in range(0,num_chars):
        current_password=allowed_chars[character]+pw_buffer
        if position>1:
            if iterate_over_position(position-1,current_password):
                return True
        else:
            #print(current_password)
            if compare(current_password):
                return True
    return False

def crack(sha256_password):
    global searched_password
    searched_password=sha256_password
    for position in range(minlength,maxlength+1):
        if iterate_over_position(position,""):
            return True
    return False

password="9f9aca7eb962d89a51b2bd1b6259d417af3f9e060469e1ab559bd428b1c619ab" # denic
if crack(password):
    print ("Passwort gefunden: "+str(found_password))
else:
    print ("Passwort leider nicht gefunden")
