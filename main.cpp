#include <stdio.h>
#include <stdlib.h>
#include <ppl7.h>
#include <ppl7-crypto.h>


class PwCrack
{
    private:
        int maxlength;
        int num_chars;
        ppl7::ByteArray allowed_chars;
        ppl7::String encrypted_pw;

        ppl7::ByteArray pw_buffer;

        bool compare(const ppl7::ByteArrayPtr &check_pw);
        bool iterate_over_position(int position);

    public:
        PwCrack();
        void setMaxLength(int length);
        void setAllowedChars(const ppl7::String &chars);


        bool crack(const ppl7::String &encrypted_pw);

        ppl7::String password() const;
};


PwCrack::PwCrack()
{
    setMaxLength(5);
    setAllowedChars("abcdefghijklmnopqrstuvwxyz");

}

void PwCrack::setMaxLength(int length)
{
    maxlength=length;
    pw_buffer.calloc(length+1);
}

void PwCrack::setAllowedChars(const ppl7::String &chars)
{
    allowed_chars=chars;
    num_chars=(int)allowed_chars.size();
}


bool PwCrack::compare(const ppl7::ByteArrayPtr &check_pw)
{
    ppl7::ByteArray ba=ppl7::Digest::sha256(check_pw);
    ppl7::String p=ba.toHex();
    //printf("checking: >>%s<< %d, p=%s <=> %s\n",(const char*)check_pw,check_pw.size(),(const char*)p,(const char*)encrypted_pw);
    if (p==encrypted_pw) {
        printf ("Passwort gefunden, es lautet: %s\n",(const char*)check_pw);
        exit(1);
        return true;
    }
    return false;
}

ppl7::String PwCrack::password() const
{
    return ppl7::String(pw_buffer);
}

bool PwCrack::iterate_over_position(int position)
{
    for (int c=0;c<num_chars;c++) {
        pw_buffer.set(position,allowed_chars[c]);
        if (position>0) {
            if (iterate_over_position(position-1)) return true;
        } else {
            if (compare(pw_buffer)) return true;
        }
    }
    return false;
}

bool PwCrack::crack(const ppl7::String &encrypted_pw)
{
    this->encrypted_pw=encrypted_pw;
    for (int length=4;length<=maxlength;length++) {
        pw_buffer.calloc(length);
        if (iterate_over_position(length-1)) return true;
    }
    return false;
}


int main(int argc, char **argv)
{

    ppl7::String password="9f9aca7eb962d89a51b2bd1b6259d417af3f9e060469e1ab559bd428b1c619ab"; // denic
    //ppl7::String password="7273854d0e9b34a60907bdde8293415a0f6edd6b8b1ef3957fcabd584be869a2"; // dcba
    PwCrack cracker;
    cracker.setMaxLength(5);
    cracker.setAllowedChars("abcdefghijklmnopqrstuvwxyz");
    if (cracker.crack(password)) {
        printf ("Passwort gefunden, es lautet: %s\n",(const char*)cracker.password());
    } else {
        printf ("Passwort nicht gefunden!\n");
    }
    return 0;
}