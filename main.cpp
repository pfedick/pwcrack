#include <stdio.h>
#include <stdlib.h>
#include <ppl7.h>
#include <math.h>
#include <ppl7-crypto.h>
#include "sha256.h"

class PwCrack
{
    private:
        uint64_t chances;
        uint64_t tries;
        double start_time;
        int progress;

        int minlength;
        int maxlength;
        int num_chars;
        ppl7::ByteArray allowed_chars;
        ppl7::String encrypted_pw;
        ppl7::ByteArray search_ba;

        ppl7::ByteArray pw_buffer;
        ppl7::ByteArray digest_buffer;

        bool compare(const ppl7::ByteArrayPtr &check_pw);
        bool iterate_over_position(int position);

    public:
        PwCrack();
        void setLength(int min, int max);
        void setAllowedChars(const ppl7::String &chars);


        bool crack(const ppl7::String &encrypted_pw);

        ppl7::String password() const;
};


PwCrack::PwCrack()
{
    setLength(1,5);
    setAllowedChars("abcdefghijklmnopqrstuvwxyz");
    digest_buffer.malloc(32);

}

void PwCrack::setLength(int min, int max)
{
    if (min<0) throw ppl7::InvalidArgumentsException();
    if (max<min) throw ppl7::InvalidArgumentsException();
    minlength=min;
    maxlength=max;
}

void PwCrack::setAllowedChars(const ppl7::String &chars)
{
    allowed_chars=chars;
    num_chars=(int)allowed_chars.size();
}


bool PwCrack::compare(const ppl7::ByteArrayPtr &check_pw)
{
    tries++;
    int new_progress=tries*100/chances;
    if (new_progress!=progress || tries%1000000==0) {
        progress=new_progress;
        double duration=ppl7::GetMicrotime()-start_time;
        double prediction=duration/tries*chances;
        if (progress>0 || tries>100000) {
            printf ("%d%% in %0.3fs, runtime prediction: %0.3fs, time left: %0.3fs            \r", progress, duration, prediction,prediction-duration);
        } else {
            printf ("...waiting for progress...       \r");
        }
        fflush(stdout);
    }
    SHA256_hash(check_pw.ptr(),check_pw.size(),(uint8_t*)digest_buffer.ptr());

    //ppl7::ByteArray ba=ppl7::Digest::sha256(check_pw);
    if (digest_buffer==search_ba) {
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
    search_ba=ppl7::Hex2ByteArray(encrypted_pw);
    for (int length=minlength;length<=maxlength;length++) {
        start_time=ppl7::GetMicrotime();
        pw_buffer.calloc(length);
        tries=0;
        progress=-1;
        chances=powl(num_chars,length);

        printf("length=%d, chances=%lu\n",length, chances);
        if (iterate_over_position(length-1)) return true;
        printf("  ==> Das hat %0.3f Sekunden gedauert                                                     \n",
            ppl7::GetMicrotime()-start_time);
    }
    return false;
}


int main(int argc, char **argv)
{
    ppl7::String password="9f9aca7eb962d89a51b2bd1b6259d417af3f9e060469e1ab559bd428b1c619ab"; // denic
    //ppl7::String password="7273854d0e9b34a60907bdde8293415a0f6edd6b8b1ef3957fcabd584be869a2"; // dcba
    //ppl7::String password="5f76e84d00de1261757445d5399dc9748603cfa98be6d457b15f3e35ed64cad2"; // bk2je6ws9
    PwCrack cracker;
    cracker.setLength(1,9);
    cracker.setAllowedChars("abcdefghijklmnopqrstuvwxyz0123456789");
    double start=ppl7::GetMicrotime();
    if (cracker.crack(password)) {
        printf ("\nPasswort gefunden, es lautet: %s\n",(const char*)cracker.password());
    } else {
        printf ("\nPasswort nicht gefunden!\n");
    }
    printf("Gesamtlaufzeit: %0.3f Sekunden\n",
            ppl7::GetMicrotime()-start);
    return 0;
}