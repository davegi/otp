/*
// as is
//keygen 256 > key.key
encode plain.txt key.key // cipher.txt
decode cipher.txt key.key > new_plain.txt
diff plain.txt new_plain.txt // nothing

// generate key
keygen 512 > key.key 
// encode w/client & server
cat plain.txt | encode_c key.key > { key, plain_text }
encode_s key.key < { key, plain_text } > cipher.txt
// encode w/c/s pipeline//
cat plain.txt | encode_c key.key | encode_s > cipher.txt

cat cipher.txt | decode_c key.key > { key, plain_text }
decode_s key.key < { key, cipher_text } > new_plain.txt

cat new_plain.txt | decode_c key.key | decode_s > new_cipher.txt

// grand finale
cat plain.txt | encode_c key.key | encode_s | decode_c key.key | decode_s | diff plain.txt // empty 


./otp -a encode key.key ---> SUCCESS 
./otp -a decode key.key ---> SUCCESS 
./otp encode/decode key.key ---> error ---> usage error
./otp -a encode/decode ----> error ---> "usage: ./otp requires key file"
 */

// encode key.key < plaintext.txt > ciphertext.txt