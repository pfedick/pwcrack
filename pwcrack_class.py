#!/usr/bin/env python
"""Brute force based password cracker"""
from hashlib import sha256

class PWCracker:
    def __init__(self):
        self.allowed_chars="abcdefghijklmnopqrstuvwxyz"
        self.num_chars=len(self.allowed_chars)
        self.minlength=1
        self.maxlength=5
        self.searched_password=None
        self.found_password=None

    def setLength(self, min, max):
        if min<1:
            raise ValueError()
        if max<min:
            raise ValueError()
        self.minlength=min
        self.maxlength=max

    def setAllowedChars(self, chars):
        self.allowed_chars=chars
        self.num_chars=len(self.allowed_chars)

    def get_password(self):
        return self.found_password

    def compare(self, password_try):
        encrypted_try=sha256(bytes(password_try,"utf-8")).hexdigest()
        if self.searched_password == encrypted_try:
            self.found_password=password_try
            return True
        return False


    def iterate_over_position(self, position, pw_buffer):
        for character in range(0,self.num_chars):
            current_password=self.allowed_chars[character]+pw_buffer
            if position>1:
                if self.iterate_over_position(position-1,current_password):
                    return True
            else:
                #print(current_password)
                if self.compare(current_password):
                    return True
        return False

    def crack(self, sha256_password):
        self.searched_password=sha256_password
        for position in range(self.minlength,self.maxlength+1):
            if self.iterate_over_position(position,""):
                return True
        return False

password="9f9aca7eb962d89a51b2bd1b6259d417af3f9e060469e1ab559bd428b1c619ab" # denic

cracker=PWCracker()
cracker.setLength(1,5)
cracker.setAllowedChars("abcdefghijklmnopqrstuvwxyz")

if cracker.crack(password):
    print ("Passwort gefunden: "+str(cracker.get_password()))
else:
    print ("Passwort leider nicht gefunden")
